#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<T> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, numeric_limits<T>::max()) {}

    pair<T, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        T max_w = numeric_limits<T>::min();
        int t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == numeric_limits<T>::max()) return {numeric_limits<T>::max(), -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    bool connected(int u, int v) {
        return u == v || path_max(u, v).second != -1;
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
        weight[v] = numeric_limits<T>::max();
    }

    bool add(int u, int v, T w) {
        if (u == v) return false;

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == numeric_limits<T>::max();
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

    bool remove(int u, int v, T w) {
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

    vector<int> u(m), v(m);
    vector<long long> w(m);
    for (int i = 0; i < m; i++) {
        cin >> u[i] >> v[i] >> w[i];
        u[i]--;
        v[i]--;
        w[i] = w[i] * m + i;
    }

    AntiMonopolyTree<long long> amt(n);
    for (int i = 0; i < n - 1; i++) amt.add(u[i], v[i], w[i]);

    vector<int> order(m - n + 1);
    iota(order.begin(), order.end(), n - 1);
    sort(order.begin(), order.end(), [&](int i, int j) { return w[i] < w[j]; });

    vector<pair<int, int>> swaps;
    for (int i : order) {
        auto [max_w, t] = amt.path_max(u[i], v[i]);

        if (w[i] < max_w) {
            swaps.emplace_back(max_w % m, i);
            amt.add(u[i], v[i], w[i]);
        }
    }

    cout << swaps.size() << "\n";
    for (auto [a, b] : swaps) cout << a + 1 << " " << b + 1 << "\n";
}
