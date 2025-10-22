#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }
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
