#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
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

    auto primes = sieve(1e6);

    int t;
    cin >> t;

    while (t--) {
        long long A, B;
        cin >> A >> B;

        vector<long long> pf(B - A + 1);
        iota(pf.begin(), pf.end(), A);
        vector<int> power(B - A + 1, 0);

        for (int p : primes)
            for (auto i = A - A % p; i <= B; i += p)
                if (i >= A)
                    while (!(pf[i - A] % p)) {
                        pf[i - A] /= p;
                        power[i - A]++;
                    }

        vector<int> count(40, 0);
        for (int i = 0; i <= B - A; i++) {
            if (pf[i] > 1) power[i]++;
            count[power[i]]++;
        }

        int size = *max_element(power.begin(), power.end()) * 2 + 1;
        vector<long long> dp(size, 0);
        dp[0] = 1;
        for (int i = 0; i <= size / 2; i++) {
            if (!count[i]) continue;

            vector<long long> temp(size);
            for (int j = 0; j < size; j++)
                temp[j] = (mul(dp[j], pow(2, count[i] - 1, MODULO), MODULO) + mul(dp[i ^ j], pow(2, count[i] - 1, MODULO), MODULO)) % MODULO;

            dp = temp;
        }

        cout << dp[0] - !count[1] << "\n";
    }
}
