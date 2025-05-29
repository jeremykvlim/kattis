#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> p(n);
    for (auto &pi : p) {
        cin >> pi;
        pi /= 100;
    }

    if (n <= 1e3) {
        vector<double> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 0; i < n; i++) {
            auto q = 1 - p[i];
            for (int k = i + 1; k; k--) dp[k] = dp[k] * q + dp[k - 1] * p[i];
            dp[0] *= q;
        }
        for (auto pr : dp) cout << fixed << setprecision(6) << pr << " ";
        exit(0);
    }

    double mean = 0, variance = 0;
    for (auto pi : p) {
        mean += pi;
        variance += pi * (1 - pi);
    }
    auto standard_deviation = sqrt(variance);

    auto cdf = [&](double x) {
        return 0.5 * (1 + erf((x - mean) / (standard_deviation * sqrt(2))));
    };
    for (double k = 0; k <= n; k++) cout << fixed << setprecision(6) << cdf(k + 0.5) - cdf(k - 0.5) << " ";
}
