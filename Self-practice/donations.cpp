#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double x, y, z, c, r;
    cin >> n >> x >> y >> z >> c >> r;

    vector<vector<double>> dp(2, vector<double>(n + 1, 0.0));
    dp[0][0] = x;
    for (int i = 0; i < n; i++) {
        dp[(i + 1) % 2] = vector<double>(n + 1, 0.0);
        for (int j = i; j <= n; j++)
            for (int k = j; k <= n; k++) {
                auto donate = (k - j) * y;
                dp[(i + 1) % 2][k] = max(dp[(i + 1) % 2][k], (dp[i % 2][j] - donate + max(0.0, donate - z) * c / 100.0) * (1.0 + r / 100.0));
            }
    }

    cout << fixed << setprecision(6) << dp[n % 2][n] << endl;
}
