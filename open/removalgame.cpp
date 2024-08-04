#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> s(2 * n + 1);
        for (int i = 1; i <= n; i++) {
            cin >> s[i];

            s[i + n] = s[i];
        }

        vector<vector<int>> g(2 * n + 1, vector<int>(2 * n + 1));
        for (int i = 1; i <= 2 * n; i++)
            for (int j = 1; j <= 2 * n; j++) g[i][j] = __gcd(s[i], s[j]);

        vector<vector<int>> dp(2 * n + 1, vector<int>(2 * n + 1, 0));
        for (int i = 3; i <= 2 * n; i++)
            for (int j = 1; j <= 2 * n - i + 1; j++) {
                int l = j, r = j + i - 1;
                dp[l][r] = 1e9;
                for (int k = l + 1; k < r; k++) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k][r] + g[l][r]);
            }

        int cost = INT_MAX;
        for (int i = 1; i <= n; i++)
            for (int j = i + 1; j <= n; j++) cost = min(cost, dp[i][j] + g[i][j] + dp[j][n + i]);
        
        cout << cost << "\n";
    }
}
