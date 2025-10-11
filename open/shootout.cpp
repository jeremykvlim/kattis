#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n;
        cin >> n;

        vector<double> p(n);
        for (auto &pi : p) {
            cin >> pi;

            pi /= 1e2;
        }

        vector<vector<vector<double>>> memo(1 << n, vector<vector<double>>(n, vector<double>(13, 0)));
        auto dp = [&](auto &&self, int mask, int turn) -> vector<double>& {
            if (any_of(memo[mask][turn].begin(), memo[mask][turn].end(), [&](auto v) { return v != 0; })) return memo[mask][turn];

            if (popcount((unsigned) mask) == 1) {
                for (int i = 0; i < n; i++)
                    if ((mask >> i) & 1) memo[mask][turn][i] = 1;
                return memo[mask][turn];
            }

            auto next = [&](int mask, int i) {
                for (int j = (i + 1) % n;; ++j %= n)
                    if ((mask >> j) & 1) return j;
            };

            vector<int> order;
            int t = turn;
            do {
                order.emplace_back(t);
                t = next(mask, t);
            } while (t != turn);

            int m = order.size();
            vector<double> pref(m + 1, 1);
            for (int i = 0; i < m; i++) pref[i + 1] = pref[i] * (1 - p[order[i]]);

            for (int i = 0; i < m; i++) {
                t = order[i];

                int count = 0;
                auto highest = -1.;
                vector<double> sum(13, 0);
                for (int j = 0; j < n; j++)
                    if (j != t && ((mask >> j) & 1)) {
                        auto temp = self(self, mask & ~(1 << j), next(mask & ~(1 << j), t));
                        if (highest + 1e-9 < temp[t]) {
                            highest = temp[t];
                            sum = temp;
                            count = 1;
                        } else if (fabs(highest - temp[t]) <= 1e-9) {
                            for (int k = 0; k < n; k++) sum[k] += temp[k];
                            count++;
                        }
                    }

                auto w = (pref[i] * p[t]) / (1 - pref[m]);
                for (int j = 0; j < n; j++) memo[mask][turn][j] += w * sum[j] / count;
            }
            return memo[mask][turn];
        };
        auto prob = dp(dp, (1 << n) - 1, 0);
        for (int i = 0; i < n; i++) cout << fixed << setprecision(9) << prob[i] * 1e2 << " ";
        cout << "\n";
    }
}
