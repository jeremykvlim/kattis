#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    vector<double> len(n - 1, 0), angle(n, 0);
    angle[0] = 180;
    for (int i = 0; i < n - 1; i++) {
        len[i] = hypot(x[i + 1] - x[i], y[i + 1] - y[i]);

        if (i < n - 2) {
            auto x1 = x[i + 1] - x[i], y1 = y[i + 1] - y[i], x2 = x[i + 2] - x[i + 1], y2 = y[i + 2] - y[i + 1];
            angle[i + 1] = 180 - abs(atan2(x1 * y2 - x2 * y1, x1 * x2 + y1 * y2)) / M_PI * 180;
        }
    }

    vector<vector<vector<double>>> dp(n - 1, vector<vector<double>>(k + 1, vector<double>(2, DBL_MAX)));
    dp[0][0][0] = 0;
    dp[0][0][1] = len[0] / 180;
    for (int i = 1; i < n - 1 && angle[i] >= 180 - 1e-9; i++) {
        dp[i][0][0] = dp[i - 1][0][1];
        dp[i][0][1] = dp[i][0][0] + len[i] / 180;
    }

    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j <= k; j++) {
            double speed = 180, dist = 0;
            for (int h = i; h; h--) {
                speed = min(speed, angle[h]);
                dist += len[h];

                dp[i][j][1] = min(dp[i][j][1], min(dp[h - 1][j - 1][1] + dist / speed, dp[h - 1][j - 1][0] + (dist + len[h - 1]) / speed));
                dp[i][j][0] = min(dp[i][j][0], min(dp[h - 1][j - 1][1] + (dist - len[i]) / speed, dp[h - 1][j - 1][0] + (dist - len[i] + len[h - 1]) / speed));
            }

            dp[i][j][1] = min(dp[i][j][1], dp[i][j - 1][0] + len[i] / 180);
        }
    }

    auto time = DBL_MAX;
    for (int i = 0; i <= k; i++) time = min(time, dp[n - 2][i][1]);

    cout << fixed << setprecision(6) << time;
}
