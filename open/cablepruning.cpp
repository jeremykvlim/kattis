#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    vector<int> degree(n + 1, 0);
    for (int _ = 0; _ < n; _++) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
        degree[u]++;
        degree[v]++;
    }

    queue<int> q;
    for (int v = 1; v <= n; v++)
        if (!--degree[v]) q.emplace(v);

    vector<bool> cyclic(n + 1, true);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        cyclic[v] = false;
        for (auto [u, w] : adj_list[v])
            if (cyclic[u])
                if (!--degree[u]) q.emplace(u);
    }

    int s = 1;
    for (; !cyclic[s]; s++);

    vector<int> cycle{s}, cycle_w;
    for (int prev = 0, v = s, next = 0;; prev = v, v = next, next = 0) {
        for (auto [u, w] : adj_list[v])
            if (cyclic[u] && u != prev) {
                next = u;
                cycle_w.emplace_back(w);
                break;
            }

        if (next == s) break;
        cycle.emplace_back(next);
    }

    int m = cycle.size();
    vector<int> root(n + 1, 0), indices(n + 1, -1);
    for (int i = 0; i < m; i++) {
        root[cycle[i]] = cycle[i];
        indices[cycle[i]] = i;
    }

    auto dfs1 = [&](auto &&self, int v, int prev, int r) -> void {
        root[v] = r;
        for (auto [u, w] : adj_list[v])
            if (u != prev && !cyclic[u]) self(self, u, v, r);
    };
    for (int v : cycle)
        for (auto [u, w] : adj_list[v])
            if (!cyclic[u]) dfs1(dfs1, u, v, v);

    mt19937_64 rng(random_device{}());
    vector<long long> subtree_xor(n + 1, 0), pref_xor(m, 0);
    while (k--) {
        int u, v;
        cin >> u >> v;

        auto h = rng();
        subtree_xor[u] ^= h;
        subtree_xor[v] ^= h;
        pref_xor[indices[root[u]]] ^= h;
        pref_xor[indices[root[v]]] ^= h;
    }

    auto total = 0LL;
    auto dfs2 = [&](auto &&self, int v, int prev = 0) -> void {
        for (auto [u, w] : adj_list[v])
            if (u != prev && !cyclic[u]) {
                self(self, u, v);
                subtree_xor[v] ^= subtree_xor[u];
                if (subtree_xor[u]) total += w;
            }
    };
    for (int v : cycle) dfs2(dfs2, v);

    unordered_map<long long, long long> groups;
    for (int i = 0; i < m; i++) {
        if (i) pref_xor[i] ^= pref_xor[i - 1];
        groups[pref_xor[i]] += cycle_w[i];
        total += cycle_w[i];
    }

    auto len = total;
    for (auto [_, sum] : groups) len = min(len, total - sum);
    cout << len;
}
