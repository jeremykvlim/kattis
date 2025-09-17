#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;

        adj_list[p].emplace_back(i);
    }

    vector<int> dp(n + 1, -1);
    auto dfs = [&](auto &&self, int v) -> int {
        if (dp[v] != -1) return dp[v];
        if (adj_list[v].empty()) return dp[v] = 1;
        
        vector<int> temp;
        for (int u : adj_list[v]) temp.emplace_back(self(self, u));
        sort(temp.rbegin(), temp.rend());
        
        dp[v] = adj_list[v].size() + 1;
        for (int i = 0; i < adj_list[v].size(); i++) dp[v] = max(dp[v], temp[i] + i);
        return dp[v];
    };
    cout << dfs(dfs, 1);
}
