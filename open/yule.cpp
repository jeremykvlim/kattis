#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> factors(n + 1), primes{2};
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; p <= n; p += 2)
        if (factors[p] == p) {
            primes.emplace_back(p);
            for (auto i = (long long) p * p; i <= n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    auto primes = sieve(sqrt(n));

    auto count = [&](auto &&self, int i = 0, long long curr = 1, int sign = 1) -> long long {
        if (i == primes.size()) return sign * n / curr;

        auto next = curr * primes[i];
        if (next > n || next * primes[i] > n) return sign * n / curr;
        return self(self, i + 1, curr, sign) + self(self, i + 1, next * primes[i], -sign);
    };
    cout << count(count);
}
