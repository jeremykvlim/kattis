#include<bits/stdc++.h>
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

__int128 pow(__int128 base, long long exponent, long long mod = LLONG_MAX) {
    __int128 value = 1;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

bool isprime(long long n) {
    if (n < 2) return false;

    mt19937_64 rng(random_device{}());
    for (int i = 0; i < 3; i++) {
        auto temp = n - 1;
        auto b = rng() % temp + 1;
        if (pow(b, temp, n) != 1) return false;

        while (!(temp & 1)) temp >>= 1;

        auto p = pow(b, temp, n);
        while (1 < p && p < n - 1) {
            p = (p * p) % n;
            temp <<= 1;
        }

        if (p == 1 && !(temp & 1)) return false;
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    auto primes = sieve(1e4);
    while (q--) {
        long long x;
        cin >> x;

        if (isprime(x + 2)) {
            cout << "2\n";
            continue;
        }

        if (x & 1) {
            cout << "-1\n";
            continue;
        }

        for (int p : primes)
            if (isprime(x + p)) {
                cout << p << "\n";
                break;
            }
    }
}
