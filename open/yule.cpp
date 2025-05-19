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
