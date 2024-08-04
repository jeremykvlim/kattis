#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int h, c;
        cin >> h >> c;

        vector<double> houses(h + 1);
        vector<vector<double>> error(h + 1, vector<double>(h + 1, 0));
        for (int i = 1; i <= h; i++) cin >> houses[i];
        for (int i = 1; i < h; i++)
            for (int j = i; j <= h; j++) {
                auto average = (houses[j] - houses[i]) / (j - i);
                for (int k = i + 1; k < j; k++) error[i][j] += fabs(average * (k - i) - (houses[k] - houses[i]));
            }

        vector<vector<double>> dp(c - 1, vector<double>(h + 1, 1e20));
        for (int i = 1; i <= h; i++) dp[0][i] = error[1][i];
        for (int i = 1; i <= c - 2; i++)
            for (int j = i + 2; j <= h; j++)
                for (int k = i + 1; k < j; k++)
                    if (dp[i][j] > dp[i - 1][k] + error[k][j] + 1e-9)
                        dp[i][j] = dp[i - 1][k] + error[k][j];

        cout << fixed << setprecision(4) << dp[c - 2][h] / h << "\n";
    }
}
