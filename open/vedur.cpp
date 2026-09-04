#include <bits/stdc++.h>
using namespace std;

struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<long long> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, LLONG_MAX) {}

    pair<long long, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        auto max_w = LLONG_MIN;
        int t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == LLONG_MAX) return {LLONG_MAX, -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
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
            if (weight[v] < weight[p]) {
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
        weight[v] = LLONG_MAX;
    }

    bool add(int u, int v, long long w) {
        if (u == v) return false;

        upward_maintain(u);
        upward_maintain(v);

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == LLONG_MAX;
        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        }

        int du = 0, dv = 0;
        while (~u && ~v) {
            if (w >= weight[u]) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w >= weight[v]) {
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
        return merged;
    }

    bool remove(int u, int v, long long w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    auto index = [&](int r, int c) {
        return (r - 1) * m + c;
    };

    int k = n * m;
    vector<long long> a(k + 1);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[index(i, j)];

    AntiMonopolyTree amt(k + 1);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            int u = index(i, j);
            if (i < n) {
                int v = index(i + 1, j);
                amt.add(u, v, max(a[u], a[v]));
            }
            if (j < m) {
                int v = index(i, j + 1);
                amt.add(u, v, max(a[u], a[v]));
            }
        }

    int q;
    cin >> q;

    while (q--) {
        int l1, d1, l2, d2;
        cin >> l1 >> d1 >> l2 >> d2;

        int u = index(l1, d1), v = index(l2, d2);
        if (u == v) cout << a[u] << "\n";
        else cout << amt.path_max(u, v).first << "\n";
    }
}