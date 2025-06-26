#include<bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << 0;
        exit(0);
    }

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<vector<array<long long, 2>>> dp(n + 1);
    vector<vector<int>> b(n + 1);
    b[0] = {INT_MIN, INT_MAX};
    auto change = LLONG_MAX;
    for (int i = 1; i <= n; i++) {
        for (int j = max(1, i - 2); j <= min(n, i + 2); j++)
            for (int k = -1; k <= 1; k++) b[i].emplace_back(a[j] + k);
        sort(b[i].begin(), b[i].end());
        b[i].erase(unique(b[i].begin(), b[i].end()), b[i].end());

        if (i == 1) {
            dp[i].resize(b[i].size());
            for (int j = 0; j < b[i].size(); j++) dp[i][j][0] = dp[i][j][1] = abs(a[i] - b[i][j]);
            continue;
        }

        dp[i].resize(max(b[i - 1].size(), b[i].size()), {(long long) 1e18, (long long) 1e18});
        for (int j = 0; j < b[i].size(); j++) {
            for (int k = 0; k < b[i - 1].size(); k++)
                if (b[i - 1][k] > b[i][j]) dp[i][j][0] = min(dp[i][j][0], dp[i - 1][k][1] + abs(a[i] - b[i][j]));
                else if (b[i - 1][k] < b[i][j]) dp[i][j][1] = min(dp[i][j][1], dp[i - 1][k][0] + abs(a[i] - b[i][j]));
                else {
                    dp[i][j][0] = min(dp[i][j][0], dp[i - 1][k][0] + abs(a[i] - b[i][j]));
                    dp[i][j][1] = min(dp[i][j][1], dp[i - 1][k][1] + abs(a[i] - b[i][j]));
                }
            if (i == n) change = min({change, dp[i][j][0], dp[i][j][1]});
        }
    }
    cout << change;
}
