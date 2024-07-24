#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double x, y, z, c, r;
    cin >> n >> x >> y >> z >> c >> r;

    vector<vector<double>> dp(2, vector<double>(n + 1, 0));
    dp[0][0] = x;
    for (int i = 0; i < n; i++) {
        dp[(i & 1) ^ 1] = vector<double>(n + 1, 0);
        for (int j = i; j <= n; j++)
            for (int k = j; k <= n; k++) dp[(i & 1) ^ 1][k] = max(dp[(i & 1) ^ 1][k], (dp[i & 1][j] - (k - j) * y + max(0.0, (k - j) * y - z) * c / 100.0) * (1.0 + r / 100.0));
    }

    cout << fixed << setprecision(6) << dp[n & 1][n];
}
