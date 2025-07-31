#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, s;
    cin >> t >> s;

    vector<double> pa(s), qa(s), pb(s), qb(s);
    for (auto &pai : pa) cin >> pai;
    for (auto &qai : qa) cin >> qai;
    for (auto &pbi : pb) cin >> pbi;
    for (auto &qbi : qb) cin >> qbi;

    vector<int> shooting(t + 1), waiting(t + 1);
    for (int t0 = 0; t0 <= t; t0++) {
        shooting[t0] = (t0 + s - 1) / s;
        waiting[t0] = (t0 <= s ? 0 : (t0 - 1) / s);
    }

    vector<vector<double>> dp_p(t + 1, vector<double>(6 * t + 1, 0)), dp_z(t + 1, vector<double>(6 * t + 1, 1));
    for (int t1 = 1; t1 <= t; t1++) {
        int pl1 = -3 * shooting[t1] + 1, pr1 = 3 * waiting[t1],
            zl1 = -3 * waiting[t1] + 1, zr1 = 3 * shooting[t1];

        for (int i = 0, t2 = t1 - 1; i < min(t1, s); i++, t2--) {
            int pl2 = -3 * shooting[t2] + 1, pr2 = 3 * waiting[t2],
                zl2 = -3 * waiting[t2] + 1, zr2 = 3 * shooting[t2];

            for (int diff = pl1; diff <= pr1; diff++) {
                auto fail = diff > zr2 ? 1. : (diff < zl2 ? 0. : dp_z[t2][3 * t + diff]),
                     shoot_2p = diff + 2 > zr2 ? 1. : (diff + 2 < zl2 ? 0. : dp_z[t2][3 * t + diff + 2]),
                     shoot_3p = diff + 3 > zr2 ? 1. : (diff + 3 < zl2 ? 0. : dp_z[t2][3 * t + diff + 3]);

                dp_p[t1][3 * t + diff] = max({dp_p[t1][3 * t + diff], pa[i] * shoot_2p + (1 - pa[i]) * fail, qa[i] * shoot_3p + (1 - qa[i]) * fail});
            }

            for (int diff = zl1; diff <= zr1; diff++) {
                auto fail = (diff > pr2 ? 1. : (diff < pl2 ? 0. : dp_p[t2][3 * t + diff])),
                     shoot_2p = (diff - 2 > pr2 ? 1. : (diff - 2 < pl2 ? 0. : dp_p[t2][3 * t + diff - 2])),
                     shoot_3p = (diff - 3 > pr2 ? 1. : (diff - 3 < pl2 ? 0. : dp_p[t2][3 * t + diff - 3]));

                dp_z[t1][3 * t + diff] = min({dp_z[t1][3 * t + diff], pb[i] * shoot_2p + (1 - pb[i]) * fail, qb[i] * shoot_3p + (1 - qb[i]) * fail});
            }
        }
    }

    cout << fixed << setprecision(6) << dp_p[t][3 * t];
}
