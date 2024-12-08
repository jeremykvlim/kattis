#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return ((unsigned __int128) x * y) % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

bool isprime(unsigned long long n) {
    if (n < 2) return false;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    vector<int> bases{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    int s = __builtin_ctzll(n - 1);
    auto d = n >> s;
    for (int a : bases) {
        auto p = pow(a % n, d, n);
        int i = s;
        while (1 < p && p < n - 1 && a % n && i--) p = mul(p, p, n);
        if (p != n - 1 && i != s) return false;
    }

    return true;
}

long long phi_inv(long long totient) {
    vector<long long> primes;
    for (int i = 1; i <= sqrt(totient); i++)
        if (!(totient % i)) {
            if (isprime(i + 1)) primes.emplace_back(i + 1);
            if (totient / i != i && isprime(totient / i + 1)) primes.emplace_back(totient / i + 1);
        }
    sort(primes.begin(), primes.end());

    auto dfs = [&](auto &&self, int i, auto n, auto m, long long phi) {
        if (phi == 1) return min(n, m);
        if (!~i) return n;

        n = self(self, i - 1, n, m, phi);
        if (!(phi % (primes[i] - 1))) {
            phi /= primes[i] - 1;
            while (!(phi % primes[i])) phi /= primes[i];
            n = self(self, i - 1, n, m / (primes[i] - 1) * primes[i], phi);
        }

        return n;
    };
    return dfs(dfs, primes.size() - 1, LLONG_MAX, totient, totient);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long totient;
    cin >> totient;

    if (totient == 1) cout << 1;
    else if (totient & 1) cout << -1;
    else {
        auto n = phi_inv(totient);
        cout << (n == LLONG_MAX ? -1 : n);
    }
}
