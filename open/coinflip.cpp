#include <bits/stdc++.h>
using namespace std;

double normal_cdf(double z) {
    return 0.5 * erfc(-z / sqrt(2));
}

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

    double mu = 0, variance = 0;
    for (auto pi : p) {
        mu += pi;
        variance += pi * (1 - pi);
    }
    auto sigma = sqrt(variance);
    for (double k = 0; k <= n; k++) cout << fixed << setprecision(6) << normal_cdf((k + 0.5 - mu) / sigma) - normal_cdf((k - 0.5 - mu) / sigma) << " ";
}
