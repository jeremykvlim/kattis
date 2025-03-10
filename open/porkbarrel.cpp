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

    void modify(int i, const int &v, const int &pos) {
        roots.emplace_back(modify(roots[i], v, pos, 1, n));
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

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip;
        pair<int, int> base, aggregate;

        SplayNode(int v = 0, int i = -1) : family{0, 0, 0}, flip(false), base{v, i}, aggregate{v, i} {}
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n + 1) {}

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].aggregate = max({ST[i].base, ST[l].aggregate, ST[r].aggregate});
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = ST[i].family;
        swap(l, r);
        ST[i].flip ^= true;
    }

    void push(int i) {
        if (!i) return;
        if (ST[i].flip) {
            auto [l, r, p] = ST[i].family;
            if (l) flip(l);
            if (r) flip(r);
            ST[i].flip = false;
        }
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) {return ST[parent].family[1] == i;};

        auto rotate = [&](int i) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) ST[k].family[child(j, k)] = i;

            int c = child(i, j), s = ST[j].family[c] = ST[i].family[c ^ 1];
            if (s) ST[s].family[2] = j;

            ST[i].family[c ^ 1] = j;
            ST[i].family[2] = k;
            ST[j].family[2] = i;
            pull(j);
        };

        auto propagate = [&](auto &&self, int i) -> void {
            if (!root(i)) self(self, ST[i].family[2]);
            push(i);
        };

        propagate(propagate, i);
        while (!root(i)) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) rotate(child(i, j) != child(j, k) ? i : j);
            rotate(i);
        }
        pull(i);
    }

    int subtree_min(int i) {
        while (ST[i].family[0]) {
            push(i);
            i = ST[i].family[0];
        }
        push(i);
        return i;
    }
};

struct LinkCutTree : SplayTree {
    LinkCutTree(int n) : SplayTree(n) {}

    void access(int i) {
        for (int u = 0, v = i; v; u = v, v = ST[v].family[2]) {
            splay(v);
            ST[v].family[1] = u;
            pull(v);
        }
        splay(i);
    }

    int find(int i) {
        access(i);
        i = subtree_min(i);
        splay(i);
        return i;
    }

    void reroot(int i) {
        access(i);
        flip(i);
        pull(i);
    }

    void link(int i, int j) {
        reroot(i);
        ST[i].family[2] = j;
    }

    void cut(int i) {
        access(i);
        ST[i].family[0] = ST[ST[i].family[0]].family[2] = 0;
        pull(i);
    }

    void cut(int i, int j) {
        reroot(i);
        cut(j);
    }

    pair<int, int> path_max(int i, int j) {
        reroot(i);
        access(j);
        return ST[j].aggregate;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<array<int, 3>> edges(m + 1);
        vector<int> weights(m + 1);
        for (int i = 1; i <= m; i++) {
            cin >> edges[i][0] >> edges[i][1] >> edges[i][2];

            weights[i] = edges[i][2];
        }
        sort(edges.begin() + 1, edges.end(), [](auto a1, auto a2) {return a1[2] > a2[2];});
        sort(weights.begin() + 1, weights.end());

        LinkCutTree lct(m + n);
        for (int i = 1; i <= m; i++) lct[i + n] = {edges[i][2], i};

        vector<int> offset(m + 1, 0);
        PersistentSegmentTree pst(m);
        for (int i = 1; i <= m; i++) {
            auto [xi, yi, wi] = edges[i];
            pst.modify(i - 1 + offset[i - 1], wi, m - i + 1);

            if (lct.find(xi) == lct.find(yi)) {
                int j = lct.path_max(xi, yi).second;

                auto [xj, yj, wj] = edges[j];
                lct.cut(j + n, xj);
                lct.cut(j + n, yj);
                pst.modify(i + offset[i]++, -wj, m - j + 1);
            }

            lct.link(i + n, xi);
            lct.link(i + n, yi);
            if (i < m) offset[i + 1] += offset[i];
        }

        int q;
        cin >> q;

        int c = 0;
        while (q--) {
            int l, h;
            cin >> l >> h;

            int i = m - (lower_bound(weights.begin() + 1, weights.end(), l - c) - weights.begin() - 1);
            c = pst.range_query(i + offset[i], 1, upper_bound(weights.begin() + 1, weights.end(), h - c) - weights.begin()).sum;
            cout << c << "\n";
        }
    }
}
