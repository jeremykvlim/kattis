#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    vector<string> r(n);
    for (auto &ri : r) {
        char _;
        cin >> _ >> ri;
    }

    vector<int> c(10);
    for (int &ci : c) cin >> ci;

    long long p10 = p;
    vector<vector<vector<long long>>> dp(10, vector<vector<long long>>(10, vector<long long>(10, 1e18)));
    for (int i = 0; i < 10; i++) {
        for (int d1 = 0; d1 < 10; d1++) {
            if (!d1) {
                for (int k = 0; k < 10; k++) dp[i][d1][k] = 0;
                continue;
            }

            dp[i][d1][0] = p10 * d1;
            for (int k = 1; k < 10; k++) {
                auto cost = dp[i][d1][k - 1];
                for (int d2 = 0; d2 < d1; d2++) cost = min({cost, dp[i][d2][k - 1] + c[d1 - d2], dp[i][d2][k] + p10 * (d1 - d2)});
                dp[i][d1][k] = cost;
            }
        }
        p10 *= 10;
    }

    auto cost = 0LL;
    for (auto ri : r) {
        vector<long long> sum(10, 0);
        for (int i = 0; i < ri.size(); i++) {
            int d = ri[ri.size() - 1 - i] - '0';
            for (int k = 0; k < 10; k++) sum[k] += dp[i][d][k];
        }

        for (int k = 1; k < 10; k++) sum[0] = min(sum[0], (long long) p * k + sum[k]);
        cost += sum[0];
    }
    cout << cost;
}
