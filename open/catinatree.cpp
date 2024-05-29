#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d;
    cin >> n >> d;

    vector<vector<int>> adj_list(n);
    for (int i = 1; i < n; i++) {
        int x;
        cin >> x;

        adj_list[x].emplace_back(i);
    }

    vector<pair<int, int>> dp(n, {1, 0});
    auto dfs = [&](auto &&self, int v = 0) -> void {
        for (int u : adj_list[v]) {
            self(self, u);

            dp[v].first += dp[u].first;

            if (dp[v].second + dp[u].second + 1 < d) {
                dp[v].first--;
                dp[v].second = max(dp[v].second, dp[u].second + 1);
            } else dp[v].second = min(dp[v].second, dp[u].second + 1);
        }
    };

    dfs(dfs);
    cout << dp[0].first;
}
