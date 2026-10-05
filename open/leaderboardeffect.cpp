#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, T;
    cin >> n >> T;

    vector<int> r(n), c(n);
    vector<double> p(n);
    for (int i = 0; i < n; i++) cin >> r[i] >> c[i] >> p[i];

    auto lsb = [&](int x) {
        return x & -x;
    };

    int m = 1 << n;
    vector<double> solved(n, 0), sum(m, 0);
    vector<vector<double>> add(T + 1, vector<double>(n, 0)), dp(T + 1, vector<double>(m, 0));
    dp[0][m - 1] = 1;
    for (int t = 0; t <= T; t++) {
        for (int i = 0; i < n; i++) solved[i] += add[t][i];

        for (int m1 = 1; m1 < m; m1++) {
            int bit = lsb(m1), m2 = m1 ^ bit, i = countr_zero((unsigned) bit);
            sum[m1] = sum[m2] + solved[i];
        }

        for (int m1 = 1; m1 < m; m1++)
            if (dp[t][m1])
                for (int m3 = m1; m3; m3 &= m3 - 1) {
                    int bit = lsb(m3), m2 = m1 ^ bit, i = countr_zero((unsigned) bit);
                    auto pr = dp[t][m1] * (sum[m1] ? solved[i] / sum[m1] : 1. / popcount((unsigned) m1));
                    if (t + r[i] <= T) dp[t + r[i]][m2] += pr * (1 - p[i]);
                    if (t + r[i] + c[i] <= T) {
                        pr *= p[i];
                        dp[t + r[i] + c[i]][m2] += pr;
                        add[t + r[i] + c[i]][i] += pr;
                    }
                }
    }
    for (auto pr : solved) cout << fixed << setprecision(6) << pr << "\n";
}