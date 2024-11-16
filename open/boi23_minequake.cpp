#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> subtree_size(n + 1, 1);
    vector<long long> dp(n + 1, 0);
    auto dfs1 = [&](auto &&self, int v = 1, int prev = -1) -> void {
        for (int u : adj_list[v])
            if (u != prev) {
                self(self, u, v);
                dp[v] += dp[u] + subtree_size[u] + 2LL * (subtree_size[v] - 1) * subtree_size[u];
                subtree_size[v] += subtree_size[u];
            }
    };
    dfs1(dfs1);

    auto spilled = LLONG_MAX;
    auto dfs2 = [&](auto &&self, int v = 1, int prev = -1) -> void {
        spilled = min(spilled, dp[v]);
        for (int u : adj_list[v])
            if (u != prev) {
                int temp1 = subtree_size[u], temp2 = subtree_size[v];
                auto temp3 = dp[u], temp4 = dp[v];
                
                subtree_size[v] -= subtree_size[u];
                dp[v] -= dp[u] + subtree_size[u] + 2LL * (subtree_size[v] - 1) * subtree_size[u];
                dp[u] += dp[v] + subtree_size[v] + 2LL * (subtree_size[u] - 1) * subtree_size[v];
                subtree_size[u] += subtree_size[v];
                self(self, u, v);
                
                tie(subtree_size[u], subtree_size[v], dp[u], dp[v]) = tie(temp1, temp2, temp3, temp4);
            }
    };
    dfs2(dfs2);
    cout << spilled;
}
