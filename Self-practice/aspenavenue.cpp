#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, w;
    cin >> n >> l >> w;

    vector<double> positions(n);
    for (auto &p : positions) cin >> p;
    sort(positions.begin(), positions.end());

    vector<vector<double>> dp(n / 2 + 1, vector<double>(n / 2 + 1));
    auto between = (double) l / (n / 2 - 1);
    for (int i = 1; i <= n / 2; i++) {
        dp[i][0] = dp[i - 1][0] + hypot((i - 1) * between - positions[i - 1], 0);
        dp[0][i] = dp[0][i - 1] + hypot((i - 1) * between - positions[i - 1], w);
    }

    for (int i = 1; i <= n / 2; i++)
        for (int j = 1; j <= n / 2; j++)
            dp[i][j] = min(dp[i - 1][j] + hypot((i - 1) * between - positions[i + j - 1], 0), dp[i][j - 1] + hypot((j - 1) * between - positions[i + j - 1], w));

    cout << fixed << setprecision(6) << dp[n / 2][n / 2];
}
