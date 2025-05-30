#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> h(n);
    for (int i = 0; i < n; i++) cin >> h[i];

    vector<vector<vector<double>>> dp(1 << n, vector<vector<double>>(n, vector<double>(n, 0)));
    vector<vector<int>> target(1 << n, vector<int>(n, -1));
    for (int m1 = 1; m1 < 1 << n; m1++) {
        int c = popcount((unsigned) m1);
        if (c == 1) {
            int k = countr_zero((unsigned) m1);
            dp[m1][k][k] = 1;
            continue;
        }

        vector<int> remaining;
        for (int i = 0; i < n; i++)
            if ((m1 >> i) & 1) remaining.emplace_back(i);

        for (int i : remaining) {
            double pr = -1;
            for (int j : remaining)
                if (j != i) {
                    int m2 = m1 ^ (1 << j), k;
                    for (int l = (i + 1) % n;; ++l %= n)
                        if ((m2 >> l) & 1) {
                            k = l;
                            break;
                        }

                    auto p = dp[m2][k][i];
                    if (pr < p) {
                        pr = p;
                        target[m1][i] = j;
                    }
                }
        }

        for (int j : remaining) {
            int s = remaining.size(), o = 0;
            vector<int> seq(s);
            for (int m = 0; m < s; m++)
                if (remaining[m] == j) {
                    o = m;
                    break;
                }
            for (int m = 0; m < s; m++) seq[m] = remaining[(m + o) % s];

            double p_all_miss = 1, p_all_prev_miss = 1;
            for (int i : seq) p_all_miss *= 1. - h[i];
            for (int i : seq) {
                int m2 = m1 ^ (1 << target[m1][i]), k;
                for (int l = (i + 1) % n;; ++l %= n)
                    if ((m2 >> l) & 1) {
                        k = l;
                        break;
                    }
                for (int l = 0; l < n; l++) dp[m1][j][l] += dp[m2][k][l] * p_all_prev_miss * h[i] / (1. - p_all_miss);
                p_all_prev_miss *= 1. - h[i];
            }
        }
    }

    for (int i = 0; i < n; i++) cout << fixed << setprecision(10) << dp.back()[0][i] << "\n";
}
