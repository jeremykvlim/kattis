#include <bits/stdc++.h>
using namespace std;

struct PersistentSegmentTree {
    struct Segment {
        int sum;

        Segment(int s = 0) : sum(s) {}

        auto operator+=(const Segment &seg) {
            sum += seg.sum;
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }

        friend auto operator+(const Segment &s, const int &v) {
            return Segment(s.sum + v);
        }
    };

    int n;
    vector<int> roots;
    vector<Segment> ST;
    vector<pair<int, int>> children;

    PersistentSegmentTree(int n) : n(n), roots{0}, ST(1), children{{0, 0}} {}

    int modify(int i, const int &v, const int &pos) {
        roots.emplace_back(modify(roots[i], v, pos, 1, n));
        return roots.size() - 1;
    }

    int modify(int i, const int &v, const int &pos, int tl, int tr) {
        if (tl + 1 == tr) {
            children.emplace_back(0, 0);
            ST.emplace_back(ST[i] + v);
            return ST.size() - 1;
        }

        auto [cl, cr] = children[i];
        int tm = tl + (tr - tl) / 2;
        if (pos < tm) cl = modify(cl, v, pos, tl, tm);
        else cr = modify(cr, v, pos, tm, tr);

        children.emplace_back(cl, cr);
        ST.emplace_back(ST[cl] + ST[cr]);
        return ST.size() - 1;
    }

    Segment range_query(int i, int l, int r) {
        return range_query(roots[i], l, r, 1, n);
    }

    Segment range_query(int i, int l, int r, int tl, int tr) {
        if (!i || r <= tl || tr <= l) return {};
        if (l <= tl && tr <= r) return ST[i];

        auto [cl, cr] = children[i];
        int tm = tl + (tr - tl) / 2;
        return range_query(cl, l, r, tl, tm) + range_query(cr, l, r, tm, tr);
    }
};

struct SegmentTree {
    struct Segment {
        int x, p, i;
        bool active;

        Segment() : x(0), p(0), i(0), active(false) {}

        auto & operator=(const array<int, 3> &v) {
            x = v[0];
            p = v[1];
            i = v[2];
            active = true;
            return *this;
        }
    };

    int n;
    vector<Segment> ST;

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void push(int i) {
        if (!ST[i].active) return;
        ST[i << 1] = ST[i << 1 | 1] = ST[i];
        ST[i].active = false;
    }

    void modify(int l, int r, const array<int, 3> &v) {
        modify(1, l, r, v, 0, n);
    }

    void modify(int i, int ql, int qr, const array<int, 3> &v, int l, int r) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            ST[i] = v;
            return;
        }

        push(i);

        int m = midpoint(l, r);
        modify(i << 1, ql, qr, v, l, m);
        modify(i << 1 | 1, ql, qr, v, m, r);
    }

    Segment query(int pos) {
        return query(1, pos, 0, n);
    }

    Segment query(int i, int pos, int l, int r) {
        if (ST[i].active) return ST[i];
        if (l + 1 == r) return {};

        int m = midpoint(l, r);
        if (pos < m) return query(i << 1, pos, l, m);
        else return query(i << 1 | 1, pos, m, r);
    }

    SegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, Q;
    cin >> n >> m >> Q;

    int k_max = 0;
    vector<int> k(m + 1);
    vector<vector<int>> seqs(m + 1);
    for (int i = 1; i <= m; i++) {
        cin >> k[i];

        k_max = max(k_max, k[i]);
        seqs[i].resize(k[i]);
        for (int &x : seqs[i]) cin >> x;
    }

    vector<vector<int>> roots(m + 1, {0});
    PersistentSegmentTree pst(k_max + 2);
    SegmentTree st(n);
    int last = 0;
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int xraw, praw;
            cin >> xraw >> praw;

            int x = xraw ^ last, p = praw ^ last;
            st.modify(p - 1, p - 1 + k[x], {x, p, (int) roots[x].size() - 1});
        } else if (q == 2) {
            int praw;
            cin >> praw;

            int p = praw ^ last;
            auto [x, p1, i, active] = st.query(p - 1);

            last = 0;
            if (active) last = (seqs[x][p - p1] + pst.range_query(roots[x][i], 1, p - p1 + 2).sum) & 255;
            cout << last << "\n";
        } else {
            int xraw, lraw, rraw;
            cin >> xraw >> lraw >> rraw;

            int x = xraw ^ last, l = lraw ^ last, r = rraw ^ last;
            roots[x].emplace_back(pst.modify(pst.modify(roots[x].back(), 1, l), -1, r + 1));
        }
    }
}
