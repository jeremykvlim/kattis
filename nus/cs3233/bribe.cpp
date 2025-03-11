#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, c, m;
        cin >> n >> c >> m;

        vector<int> b(n);
        vector<double> p(n);
        for (int i = 0; i < n; i++) {
            cin >> b[i] >> p[i];

            p[i] /= 100;
        }

        vector<vector<double>> dp(1 << n, vector<double>(c + 1, -2));
        auto convert = [&](auto &&self, int mask, int c, int m) {
            if (dp[mask][c] > -1) return dp[mask][c];
            if (!c) return dp[mask][c] = 1;
            if (c > popcount((unsigned) mask)) return dp[mask][c] = 0;

            double prob = 0;
            for (int i = 0; i < b.size(); i++) {
                if (!((mask >> i) & 1) || b[i] > m) continue;
                auto success = p[i] * self(self, mask ^ (1 << i), c - 1, m - b[i]), failure = (1 - p[i]) * self(self, mask ^ (1 << i), c, m - b[i]);
                prob = max(prob, success + failure);
            }

            return dp[mask][c] = prob;
        };
        cout << fixed << setprecision(6) << convert(convert, (1 << n) - 1, c, m) << "\n";
    }
}
