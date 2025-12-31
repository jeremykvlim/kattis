#include <bits/stdc++.h>
using namespace std;

struct PersistentSegmentTree {
    struct Segment {
        int sum;

        Segment(int s = 0) : sum(s) {}

        auto & operator+=(const Segment &seg) {
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
    map<int, tuple<int, int, int, bool>> intervals;
    intervals[0] = intervals[n] = {0, 0, 0, false};

    auto split = [&](int pos) {
        if (pos <= 0) return intervals.begin();
        if (pos >= n) return intervals.find(n);
        auto it = prev(intervals.upper_bound(pos));
        if (it->first == pos) return it;
        return intervals.emplace(pos, it->second).first;
    };

    auto merge = [&](auto it) {
        if (it == intervals.end() || it->first == n) return;

        if (it != intervals.begin()) {
            auto temp = prev(it);
            if (temp->second == it->second) {
                intervals.erase(it);
                it = temp;
            }
        }

        auto temp = next(it);
        if (temp != intervals.end() && temp->first != n && it->second == temp->second) intervals.erase(temp);
    };

    int last = 0;
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int xraw, praw;
            cin >> xraw >> praw;

            int x = xraw ^ last, p = praw ^ last;

            auto it_l = split(p - 1), it_r = split(p - 1 + k[x]);
            intervals.erase(it_l, it_r);
            auto it = intervals.emplace(p - 1, tuple{x, p, (int) roots[x].size() - 1, true}).first;
            merge(it);
            if (it != intervals.begin()) merge(prev(it));
        } else if (q == 2) {
            int praw;
            cin >> praw;

            int p = praw ^ last;
            auto [x, p1, i, active] = prev(intervals.upper_bound(p - 1))->second;

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