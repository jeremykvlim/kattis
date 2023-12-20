#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, k;
        cin >> n >> k;

        vector<pair<int, int>> items(n);
        for (auto &p : items) cin >> p.first >> p.second;
        sort(items.begin(), items.end());

        vector<vector<long long>> dp(k + 1, vector<long long>(n + 1, INT_MIN));
        for (int i = n - 1; i >= 0; i--) dp[0][i] = max(dp[0][i + 1], (long long) items[i].first - items[i].second);
        for (int i = 1; i <= k; i++)
            for (int j = n - 1; j >= 0; j--) dp[i][j] = max({min((long long) items[j].first - items[j].second, dp[i - 1][j + 1] - items[j].second), dp[i][j + 1], (i + j > n) ? INT_MIN : dp[i][j]});

        cout << max(dp[k][0], 0LL) << "\n";
    }
}
