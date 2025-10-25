#include <bits/stdc++.h>
using namespace std;

double log_binomial_coefficient(long long n, long long k) {
    if (k < 0 || k > n) return numeric_limits<double>::quiet_NaN();
    return lgamma(n + 1) - lgamma(k + 1) - lgamma(n - k + 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    auto sum = 0.;
    for (int k = 0; k <= min(n, m); k++) sum += exp(log_binomial_coefficient(n, k) - n * log(2)) * (m - k);
    cout << fixed << setprecision(6) << sum;
}
