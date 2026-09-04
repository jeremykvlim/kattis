#include <bits/stdc++.h>
using namespace std;

struct PersistentSegmentTree {
    struct Monoid {
        int sum;

        Monoid(int s = 0) : sum(s) {}

        auto & operator+=(const Monoid &monoid) {
            sum += monoid.sum;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }

        friend auto operator+(const Monoid &monoid, const int &v) {
            return Monoid(monoid.sum + v);
        }
    };

    int n;
    vector<int> roots;
    vector<Monoid> ST;
    vector<pair<int, int>> children;

    PersistentSegmentTree(int n) : n(n), roots{0}, ST(1), children{{0, 0}} {}

    int point_update(int i, const int &v, const int &pos) {
        roots.emplace_back(point_update(roots[i], v, pos, 1, n));
        return roots.size() - 1;
    }

    int point_update(int i, const int &v, const int &pos, int tl, int tr) {
        if (tl + 1 == tr) {
            children.emplace_back(0, 0);
            ST.emplace_back(ST[i] + v);
            return ST.size() - 1;
        }

        auto [cl, cr] = children[i];
        int tm = tl + (tr - tl) / 2;
        if (pos < tm) cl = point_update(cl, v, pos, tl, tm);
        else cr = point_update(cr, v, pos, tm, tr);

        children.emplace_back(cl, cr);
        ST.emplace_back(ST[cl] + ST[cr]);
        return ST.size() - 1;
    }

    Monoid range_query(int i, int l, int r) {
        return range_query(roots[i], l, r, 1, n);
    }

    Monoid range_query(int i, int l, int r, int tl, int tr) {
        if (!i || r <= tl || tr <= l) return {};
        if (l <= tl && tr <= r) return ST[i];

        auto [cl, cr] = children[i];
        int tm = tl + (tr - tl) / 2;
        return range_query(cl, l, r, tl, tm) + range_query(cr, l, r, tm, tr);
    }
};

struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<pair<int, int>> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, {INT_MAX, -1}) {}

    pair<int, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        int max_w = INT_MIN, t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u].first == INT_MAX) return {INT_MAX, -1};
            if (max_w < weight[u].first) {
                max_w = weight[u].first;
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    void upward_maintain(int v) {
        while (~parent[v]) {
            int p = parent[v];
            if (3 * size[v] <= 2 * size[p]) {
                v = p;
                continue;
            }

            size[p] -= size[v];
            parent[v] = parent[p];
            if (weight[v].first < weight[p].first) {
                size[v] += size[p];
                swap(weight[v], weight[p]);
                parent[p] = v;
            }
        }
    }

    int root(int v) {
        while (~parent[v]) v = parent[v];
        return v;
    }

    void cut(int v) {
        for (int p = parent[v]; ~p; p = parent[p]) size[p] -= size[v];
        parent[v] = -1;
        weight[v] = {INT_MAX, -1};
    }

    int add(int u, int v, pair<int, int> w) {
        if (u == v) return w.second;

        auto [max_w, t] = path_max(u, v);
        int i = -1;
        if (max_w != INT_MAX) {
            if (w.first >= max_w) return w.second;
            i = weight[t].second;
            cut(t);
        }

        int du = 0, dv = 0;
        while (~u && ~v) {
            if (w.first >= weight[u].first) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w.first >= weight[v].first) {
                int p = parent[v];
                if (~p) size[p] += dv;
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                }

                du -= size[u];
                dv += size[u];
                size[v] += size[u];
                w = exchange(weight[u], w);
                u = exchange(parent[u], v);
                if (~u) size[u] += du;
            }
        }

        if (~v)
            for (v = parent[v]; ~v; v = parent[v]) size[v] += dv;
        return i;
    }

    bool remove(int u, int v, int w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
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
        sort(edges.begin() + 1, edges.end(), [](auto a1, auto a2) { return a1[2] > a2[2]; });
        sort(weights.begin() + 1, weights.end());

        AntiMonopolyTree amt(n + 1);
        PersistentSegmentTree pst(m);
        vector<int> version(m + 1, 0);
        for (int i = 1; i <= m; i++) {
            auto [xi, yi, wi] = edges[i];
            version[i] = pst.point_update(version[i - 1], wi, m - i + 1);

            int j = amt.add(xi, yi, {wi, i});
            if (j != -1) {
                auto [xj, yj, wj] = edges[j];
                version[i] = pst.point_update(version[i], -wj, m - j + 1);
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
