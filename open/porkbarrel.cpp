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

struct WeightedDisjointSets {
    vector<int> sets, size;
    vector<pair<int, int>> weight;

    WeightedDisjointSets(int n) : sets(n), size(n, 1), weight(n, {INT_MAX, 0}) {
        iota(sets.begin(), sets.end(), 0);
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]].first <= weight[v].first) sets[v] = sets[sets[v]];
        return sets[v];
    }

    int find(int u, int w = INT_MAX - 1) {
        while (weight[u].first <= w) u = compress(u);
        return u;
    }

    void link(int u, int v, pair<int, int> w) {
        while (u != v) {
            u = find(u, w.first);
            v = find(v, w.first);
            if (size[u] < size[v]) swap(u, v);
            size[u] += size[v];
            swap(compress(v), u);
            swap(weight[v], w);
        }
    }

    void cut(int v) {
        compress(v) = v;
        weight[v] = {INT_MAX, 0};
    }

    int path_max(int u, int v) {
        if (find(u) != find(v)) return -1;

        for (;;) {
            if (weight[u].first > weight[v].first) swap(u, v);
            if (compress(u) == v) break;
            u = compress(u);
        }
        return u;
    }

    int unite(int u, int v, pair<int, int> w) {
        if (u == v) return w.second;

        int t = path_max(u, v);
        if (t == -1) {
            link(u, v, w);
            return -1;
        } else if (weight[t].first > w.first) {
            int i = weight[t].second;
            cut(t);
            link(u, v, w);
            return i;
        }
        return w.second;
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

        WeightedDisjointSets wdsu(n + 1);
        PersistentSegmentTree pst(m);
        vector<int> version(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            auto [xi, yi, wi] = edges[i];
            version[i] = pst.modify(version[i - 1], wi, m - i + 1);

            int j = wdsu.unite(xi, yi, {wi, i});
            if (j != -1) {
                auto [xj, yj, wj] = edges[j];
                version[i] = pst.modify(version[i], -wj, m - j + 1);
            }
        }

        int q;
        cin >> q;

        int c = 0;
        while (q--) {
            int l, h;
            cin >> l >> h;

            int i = m - (lower_bound(weights.begin() + 1, weights.end(), l - c) - weights.begin() - 1);
            c = pst.range_query(version[i], 1, upper_bound(weights.begin() + 1, weights.end(), h - c) - weights.begin()).sum;
            cout << c << "\n";
        }
    }
}
