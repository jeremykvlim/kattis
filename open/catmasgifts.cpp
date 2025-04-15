#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<array<double, 2>>> dp1(n + 1, vector<array<double, 2>>(n + 1, {0, 0})), dp2(n + 1, vector<array<double, 2>>(n + 1, {0, 0}));
    dp1[1][0][0] = dp1[1][0][1] = dp2[1][0][0] = 1;
    for (int i = 2; i <= n; i++) {
        auto P = [&](int j) -> pair<double, double> {
            if (!j) return {0, 0};
            auto p = (double) j / (i - 1), q = 1 - p;
            return {p * dp1[i - 1][j - 1][1] + q * dp1[i - 1][j][0],
                    p * dp2[i - 1][j - 1][1] + q * dp2[i - 1][j][0]};
        };

        for (int j = 0; j <= i - 1; j++)
            for (int k = 0; k < 2; k++) {
                auto p = (double) j / (i - k), q = (double) (i - j - k) / (i - k);
                auto [p1, p2] = P(j - 1);
                auto [q1, q2] = P(j);
                dp1[i][j][k] = (double) i / (i - k) + p * p1 + q * q1;
                dp2[i][j][k] = p * p2 + q * q2;
            }
    }
    cout << fixed << setprecision(6) << dp1[n][n - 1][1] / dp2[n][n - 1][1];
}
