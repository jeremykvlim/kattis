#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;
    T unit;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = unit;
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func, T unit) : BIT(n, unit), f(move(func)), unit(unit) {}
};

template <typename T, typename F>
void centroid_decomposition(const vector<vector<pair<int, T>>> &adj_list, F &&f) {
    vector<int> subtree_size(adj_list.size());
    vector<bool> removed(adj_list.size(), false);
    auto decompose = [&](auto &&self, int root = 1) -> void {
        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            subtree_size[v] = 1;
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) {
                    self(self, u, v);
                    subtree_size[v] += subtree_size[u];
                }
        };
        dfs(dfs, root);

        int c = root, size = subtree_size[root];
        for (int prev = -1, next = -1;; prev = c, c = next, next = -1) {
            for (auto [u, w] : adj_list[c])
                if (u != prev && !removed[u] && 2 * subtree_size[u] > size) {
                    next = u;
                    break;
                }
            if (!~next) break;
        }
        removed[c] = true;

        f(c, removed);
        for (auto [v, w] : adj_list[c])
            if (!removed[v]) self(self, v);
    };
    decompose(decompose);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v, s;
        cin >> u >> v >> s;

        adj_list[u].emplace_back(v, s);
        adj_list[v].emplace_back(u, s);
    }

    vector<array<int, 4>> trains(k);
    for (auto &[u, v, t, d] : trains) cin >> u >> v >> t >> d;
    sort(trains.begin(), trains.end(), [&](auto &t1, auto &t2) { return t1[2] > t2[2]; });

    vector<vector<pair<int, long long>>> centroids(n + 1);
    centroid_decomposition(adj_list, [&](int c, const auto &removed) {
        auto dfs = [&](auto &&self, int v, int prev = -1, long long d = 0) -> void {
            centroids[v].emplace_back(c, d);
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) self(self, u, v, d + w);
        };
        dfs(dfs, c);
    });

    vector<vector<long long>> times(n + 1);
    for (auto [u, v, t, d] : trains)
        for (auto [c, dist] : centroids[u]) times[c].emplace_back(t - dist);

    for (int c = 1; c <= n; c++) {
        sort(times[c].begin(), times[c].end());
        times[c].erase(unique(times[c].begin(), times[c].end()), times[c].end());
    }

    vector<FenwickTree<int>> fws;
    for (int c = 0; c <= n; c++) fws.emplace_back(times[c].size() + 1, [](int x, int y) { return max(x, y); }, 0);

    for (int i = 0; i < k; i++) {
        auto [u, v, t, d] = trains[i];
        int dp = 0;
        for (auto [c, dist] : centroids[v])
            if (!times[c].empty()) {
                int j = lower_bound(times[c].begin(), times[c].end(), dist + t + d) - times[c].begin();
                if (j < times[c].size()) dp = max(dp, fws[c].range_query(times[c].size() - j));
            }

        for (auto [c, dist] : centroids[u])
            if (!times[c].empty()) {
                int j = lower_bound(times[c].begin(), times[c].end(), t - dist) - times[c].begin();
                fws[c].update(times[c].size() - j, dp + 1);
            }
    }

    for (int v = 1; v <= n; v++) {
        int x = 0;
        for (auto [c, dist] : centroids[v])
            if (!times[c].empty()) {
                int j = lower_bound(times[c].begin(), times[c].end(), dist) - times[c].begin();
                if (j < times[c].size()) x = max(x, fws[c].range_query(times[c].size() - j));
            }
        cout << x << " ";
    }
}
