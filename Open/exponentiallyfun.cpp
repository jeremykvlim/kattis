#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

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

long long pow(long long base, long long exponent, long long mod = 1) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> primes = sieve(n + 1);
    vector<long long> dp(n + 1, -1);
    dp[0] = 1;
    for (int p : primes)
        for (int i = 0; i <= n - p; i++)
            if (dp[i] != -1) dp[i + p] = (min(dp[i] * p, dp[i + p] != -1 ? dp[i + p] : LLONG_MAX)) % MODULO;

    auto smallest = dp[n] % MODULO, biggest = 1LL;
    while (n % 3) {
        biggest = (biggest << 1) % MODULO;
        n -= 2;
    }
    biggest *= pow(3, n / 3, MODULO);

    cout << smallest << " " << biggest;
}
