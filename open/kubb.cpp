#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double pa, pb, pc;
    cin >> n >> pa >> pb >> pc;

    int m = min(n, 25);
    vector<vector<unsigned>> masks(m + 1);
    for (int len = 1; len <= m; len++) {
        auto dfs = [&](auto &&self, unsigned mask = 0, int pos = 0, bool zero = false) -> void {
            if (pos == len) {
                masks[popcount(mask)].emplace_back(mask);
                return;
            }

            self(self, mask | (1 << pos), pos + 1, false);
            if (0 < pos && pos < len - 1 && !zero) self(self, mask, pos + 1, true);
        };
        dfs(dfs);
    }

    unordered_map<unsigned, double> dp;
    auto mask = [&](int len) {
        return (1 << len) - 1;
    };

    vector<double> values(m + 1);
    for (int ones = 1; ones <= m; ones++)
        for (auto m1 : masks[ones]) {
            int len = bit_width(m1);

            auto E = [&](int i) -> double {
                auto m2 = m1 ^ (1 << i);
                if (!i) return len == 1 ? 0 : dp[m2 >> (1 + !((m2 >> 1) & 1))];
                if (i == len - 1) return dp[m2 & mask(len - 1 - !((m2 >> (len - 2)) & 1))];

                bool l = (m1 >> (i - 1)) & 1, r = (m1 >> (i + 1)) & 1;
                if (l && r) return dp[m2];

                double e = 0;
                int left = i;
                auto mask_l = m2 & mask(i);
                if (!l) mask_l &= mask(--left);
                if (left) e += dp[mask_l];

                int right = len - i - 1;
                auto mask_r = m2 >> (i + 1);
                if (!r) {
                    right--;
                    mask_r >>= 1;
                }
                if (right) e += dp[mask_r];
                return e;
            };

            for (int i = 0; i < len; i++)
                if ((m1 >> i) & 1) values[i] = E(i);

            auto it = dp.emplace(m1, 1e20).first;
            for (int x = -1; x <= len; x++) {
                double numer = 1, denom = 0;

                int pos = x - 1;
                if (0 <= pos && pos < len && ((m1 >> pos) & 1)) {
                    numer += pa * values[pos];
                    denom += pa;
                }

                pos = x;
                if (0 <= pos && pos < len && ((m1 >> pos) & 1)) {
                    numer += pb * values[pos];
                    denom += pb;
                }

                pos = x + 1;
                if (0 <= pos && pos < len && ((m1 >> pos) & 1)) {
                    numer += pc * values[pos];
                    denom += pc;
                }

                if (denom) it->second = min(it->second, numer / denom);
            }
        }

    double throws;
    if (n <= m) throws = dp[mask(n)];
    else {
        auto v25 = dp[mask(25)], v24 = dp[mask(24)];
        throws = v25 + (v25 - v24) * (n - 25);
    }
    cout << fixed << setprecision(5) << throws;
}
