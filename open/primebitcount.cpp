#include <bits/stdc++.h>
using namespace std;

constexpr long long MODULO = 1e9 + 607;

long long C(long long n, long long k, int p, vector<long long> &fact, vector<long long> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return (C(n / p, k / p, p, fact, fact_inv) * C(n % p, k % p, p, fact, fact_inv)) % p;
    return (fact[n] * fact_inv[k] % p * fact_inv[n - k] % p) % p;
}

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

    int n, k;
    cin >> n >> k;

    if (k == 2) {
        cout << 1;
        exit(0);
    }

    vector<vector<vector<long long>>> dp(n + 1, vector<vector<long long>>(k, vector<long long>(k + 1, 0)));
    for (int i = 0; i < k; i++)
        for (int j = i + 1; j <= k; j++) dp[n][i][j] = 1;

    for (int h = n - 1; h; h--)
        for (int i = 0; i < k - 1; i++)
            for (int j = i + 1; j < k; j++) {
                if (i) dp[h][i][j] = dp[h + 1][i - 1][j - 1] + (h + i >= n ? 1 : dp[h + i][j - i][k - i]);
                else if (h < n) dp[h][i][j] += dp[h + 1][j - 1][k - 1] + (h < k - 1 ? dp[h + 1][0][j - 1] : 0);

                dp[h][i][j] %= MODULO;
            }

    auto count = 0LL;
    for (int i = 1; i < k; i++) count = (count + dp[i][0][k - 1]) % MODULO;

    vector<long long> fact(k + 1, 1), fact_inv(k + 1, 1);
    auto prepare = [&](vector<long long> &fact, vector<long long> &fact_inv, int mod) {
        vector<long long> inv(k + 1, 1);

        for (int i = 1; i <= k; i++) {
            if (i > 1) inv[i] = ((mod - mod / i) * inv[mod % i]) % mod;
            fact[i] = (i * fact[i - 1]) % mod;
            fact_inv[i] = (inv[i] * fact_inv[i - 1]) % mod;
        }
    };

    prepare(fact, fact_inv, MODULO);

    auto primes = sieve(k);
    for (int p : primes) {
        if (p == 2 || p == 3) continue;
        count = (count + C(k, p, MODULO, fact, fact_inv)) % MODULO;
    }

    cout << count;
}
