#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    if (n < 2) return {};

    int r = sqrt(n);
    vector<int> primes{2};
    vector<bool> composite(n / 2 + 1, false);
    for (int o = 3; o <= r; o += 2)
        if (!composite[o / 2]) {
            for (auto m = (long long) o * o; m <= n; m += 2 * o) composite[m / 2] = true;
            primes.emplace_back(o);
        }

    for (int o = (r + 1) | 1; o <= n; o += 2)
        if (!composite[o / 2]) primes.emplace_back(o);
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long m;
    cin >> m;

    int n = ceil(sqrtl(m));
    auto primes = sieve(n);

    auto safe = 4LL, limit = m;
    for (int i = 0; i < primes.size(); i++) {
        int p = primes[i];
        limit = min(limit, p * (m / p) - 1);

        int q = i + 1 < primes.size() ? primes[i + 1] : n + 1;
        safe = max(safe, min(limit, (long long) q * q - 1));
    }
    cout << safe;
}
