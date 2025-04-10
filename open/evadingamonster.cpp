#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<unordered_map<int, int>> adj_matrix(n);
    vector<set<pair<int, int>>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;

        adj_matrix[u][v] = adj_matrix[v][u] = 0;
        adj_list[u].emplace(0, v);
        adj_list[v].emplace(0, u);
    }

    vector<int> a(m + 1);
    for (int i = 1; i <= m; i++) {
        cin >> a[i];
        a[i]--;
    }

    vector<int> dp(n, 0);
    dp[a[m]] = 1e9;
    for (int i = m - 1; i >= 0; i--) {
        int t = -1, u = a[i], v = a[i + 1];
        adj_list[u].erase({adj_matrix[u][v], v});

        int moves = 1e9;
        while (!adj_list[u].empty()) {
            auto it = prev(adj_list[u].end());
            tie(moves, t) = *it;
            if (moves <= dp[u] + 1) break;
            adj_list[u].erase(it);

            if (moves != dp[t]) {
                adj_matrix[u][t] = dp[t];
                adj_list[u].emplace(dp[t], t);
                continue;
            }

            adj_matrix[u][t] = dp[u] + 1;
            adj_list[u].emplace(dp[u] + 1, t);
            dp[t] = dp[u] + 1;

            adj_list[t].erase({adj_matrix[t][u], u});
            adj_matrix[t][u] = 1e9;
            adj_list[t].emplace(1e9, u);
        }
        adj_list[v].erase({adj_matrix[v][u], u});

        dp[u] = moves = 1e9;
        while (!adj_list[v].empty()) {
            auto it = adj_list[v].begin();
            tie(moves, t) = *it;
            if (moves == dp[t]) break;
            adj_list[v].erase(it);

            adj_list[v].emplace(dp[t], t);
        }

        dp[v] = min(moves + 1, (int) 1e9);
        adj_matrix[v][u] = 1e9;
        adj_list[v].emplace(1e9, u);

        adj_matrix[u][v] = dp[v];
        adj_list[u].emplace(dp[v], v);
    }

    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> int {
        for (auto [u, w] : adj_matrix[v])
            if (u && u != prev) dp[v] = min(dp[v], self(self, u, v) + 1);
        return dp[v];
    };
    dfs(dfs);
    cout << (dp[1] != 1e9 ? dp[1] : -1);
}
