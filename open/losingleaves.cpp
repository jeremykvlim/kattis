#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> adj_list(n);
    for (int i = 1; i < n; i++) {
        int p;
        cin >> p;

        adj_list[p].emplace_back(i);
    }

    vector<int> dp(n, 0);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        int t = 0;
        for (int u : adj_list[v]) {
            self(self, u);
            if (dp[v] < dp[u]) {
                dp[v] = dp[u];
                t = u;
            }
        }
        dp[v]++;
        if (t) dp[t] = 1e9;
    };
    dfs(dfs);

    sort(dp.begin(), dp.end());
    int count = count_if(adj_list.begin(), adj_list.end(), [&](auto neighbors) { return neighbors.empty(); });
    for (int i = 0; i < n; i++)
        if (k >= dp[i]) {
            k -= dp[i];
            count--;
        }
    cout << count;
}
