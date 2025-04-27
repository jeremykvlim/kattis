#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = 0;
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, 0), f(func) {}
};

struct CentroidDecomposition {
    vector<vector<pair<int, int>>> adj_list, centroids;
    vector<bool> removed;
    vector<int> subtree_size;
    vector<vector<long long>> dist;

    CentroidDecomposition(int n) : adj_list(n), centroids(n), removed(n, false), subtree_size(n, 0) {}

    void add_edge(int u, int v, int w) {
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    void decompose(int root = 1, int layer = 0) {
        auto dfs1 = [&](auto &&self, int v, int prev = -1) -> void {
            subtree_size[v] = 1;
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) {
                    self(self, u, v);
                    subtree_size[v] += subtree_size[u];
                }
        };
        dfs1(dfs1, root);

        auto dfs2 = [&](auto &&self, int size, int v, int prev = -1) -> int {
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u])
                    if (2 * subtree_size[u] > size) return self(self, size, u, v);
            return v;
        };
        int c = dfs2(dfs2, subtree_size[root], root);
        removed[c] = true;

        if (dist.size() <= layer) dist.emplace_back(vector<long long>(adj_list.size(), 0));
        auto dfs3 = [&](auto &&self, int v, int prev = -1, long long d = 0) -> void {
            dist[layer][v] = d;
            centroids[v].emplace_back(c, layer);
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) self(self, u, v, d + w);
        };
        dfs3(dfs3, c);

        for (auto [v, w] : adj_list[c])
            if (!removed[v]) decompose(v, layer + 1);
    }

    auto & operator[](int i) {
        return centroids[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    CentroidDecomposition cd(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v, s;
        cin >> u >> v >> s;

        cd.add_edge(u, v, s);
    }
    cd.decompose();

    vector<array<int, 4>> trains(k);
    for (auto &[u, v, t, d] : trains) cin >> u >> v >> t >> d;
    sort(trains.begin(), trains.end(), [&](auto &t1, auto &t2) { return t1[2] > t2[2]; });

    vector<vector<long long>> keys(n + 1);
    for (auto [u, v, t, d] : trains)
        for (auto [c, lvl] : cd[u]) keys[c].emplace_back(t - cd.dist[lvl][u]);

    for (int c = 1; c <= n; c++) {
        sort(keys[c].begin(), keys[c].end());
        keys[c].erase(unique(keys[c].begin(), keys[c].end()), keys[c].end());
    }

    vector<FenwickTree<int>> fws;
    for (int c = 0; c <= n; c++) fws.emplace_back(keys[c].size() + 1, [&](int x, int y) { return max(x, y); });
    for (int i = 0; i < k; i++) {
        auto [u, v, t, d] = trains[i];

        int dp = 0;
        for (auto [c, layer] : cd[v]) {
            if (keys[c].empty()) continue;

            int j = lower_bound(keys[c].begin(), keys[c].end(), cd.dist[layer][v] + t + d) - keys[c].begin();
            if (j < keys[c].size()) dp = max(dp, fws[c].range_query(keys[c].size() - j));
        }

        for (auto [c, layer] : cd[u]) {
            if (keys[c].empty()) continue;

            int j = lower_bound(keys[c].begin(), keys[c].end(), t - cd.dist[layer][u]) - keys[c].begin();
            fws[c].update(keys[c].size() - j, dp + 1);
        }
    }

    for (int v = 1; v <= n; v++) {
        int x = 0;

        for (auto [c, layer] : cd[v]) {
            if (keys[c].empty()) continue;

            int j = lower_bound(keys[c].begin(), keys[c].end(), cd.dist[layer][v]) - keys[c].begin();
            if (j < keys[c].size()) x = max(x, fws[c].range_query(keys[c].size() - j));
        }
        cout << x << " ";
    }
}
