#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d;
    cin >> n >> d;

    vector<int> c(n);
    for (int &ci : c) cin >> ci;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<vector<long long>> dp(2 * d + 3, vector<long long>(n, 1e10));
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        bool leaf = false;
        for (int u : adj_list[v])
            if (u != prev) {
                leaf = true;
                self(self, u, v);
            }

        if (leaf)
            for (int i = 2 * d + 1; i; i--) {
                auto total = 0LL;
                for (int u : adj_list[v])
                    if (u != prev) total += dp[2 * d + 3 - i][u];

                for (int u : adj_list[v])
                    if (u != prev) dp[i - 1][v] = min(dp[i - 1][v], total - dp[2 * d + 3 - i][u] + dp[i][u]);

                dp[i - 1][v] = min(dp[i - 1][v], dp[i][v]);
            }

        for (int i = 1; i < 2 * d + 2; i++) {
            if (!leaf && i <= d) {
                dp[i][v] = 0;
                continue;
            }

            long long cost = c[v];
            for (int u : adj_list[v])
                if (u != prev) cost += dp[1][u];
            dp[i][v] = min(dp[i][v], cost);

            if (leaf && i <= 2 * d) {
                auto total = 0LL;
                for (int u : adj_list[v])
                    if (u != prev) total += dp[i + 1][u];
                dp[i][v] = min(dp[i][v], total);
            }
        }
    };
    dfs(dfs);

    auto cost = LLONG_MAX;
    for (int i = d + 1; i < 2 * d + 2; i++) cost = min(cost, dp[i][0]);
    cout << cost;
}
