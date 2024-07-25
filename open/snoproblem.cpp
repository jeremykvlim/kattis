#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int total = 0;
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int a, b, d;
        cin >> a >> b >> d;

        adj_list[a].emplace_back(b, d);
        adj_list[b].emplace_back(a, d);
        total += 2 * d;
    }

    int len = 0;
    vector<vector<int>> dp(n + 1, vector<int>(3));
    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        for (auto [u, w]: adj_list[v])
            if (u != prev) {
                self(self, u, v);
                len = max(len, max({dp[v][1] + dp[u][0] + w, dp[v][0] + dp[u][1] + w, dp[v][2] + dp[u][0] + w, dp[v][0] + dp[u][2], dp[v][2] + dp[u][2]}));
                dp[v][1] = max(dp[v][1], max({dp[u][1] + w, dp[v][2] + dp[u][0] + w, dp[u][2] + dp[v][0]}));
                dp[v][2] = max(dp[v][2], max(dp[u][2], dp[v][0] + dp[u][0] + w));
                dp[v][0] = max(dp[v][0], dp[u][0] + w);
            }
    };

    dfs(dfs);
    cout << total - len;
}
