#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<vector<pair<int, int>>> adj_list(m);
    for (int i = 0; i < m - 1; i++) {
        int u, v, t;
        cin >> u >> v >> t;

        adj_list[u].emplace_back(v, t);
        adj_list[v].emplace_back(u, t);
    }

    vector<vector<vector<int>>> dp(2, vector<vector<int>>(m, vector<int>(n + 1, 0)));
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (auto [u, t] : adj_list[v])
            if (u != prev) {
                self(self, u, v);
                for (int i = n; ~i; i--) {
                    int count = 0;
                    for (int j = t; j <= i; j++) count = max(count, dp[0][u][j - t] + dp[1][v][i - j]);
                    for (int j = 2 * t; j <= i; j++) count = max(count, dp[1][u][j - 2 * t] + dp[0][v][i - j]);
                    dp[0][v][i] = max(dp[0][v][i], count);
                }

                for (int i = n; ~i; i--) {
                    int count = 0;
                    for (int j = 2 * t; j <= i; j++) count = max(count, dp[1][u][j - 2 * t] + dp[1][v][i - j]);
                    dp[1][v][i] = max(dp[1][v][i], count);
                }
            }

        for (int i = 0; i <= n; i++) {
            dp[0][v][i]++;
            dp[1][v][i]++;
        }
    };
    dfs(dfs);
    cout << dp[0][0][n] - 1;
}
