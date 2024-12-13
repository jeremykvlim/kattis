#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
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
    if (n == 2 || n == 5 || n == 11) return true;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    auto miller_rabin = [&](int a) {
        int s = __builtin_ctzll(n - 1);
        auto d = n >> s, x = pow(a % n, d, n);
        if (x == 1 || x == n - 1) return true;

        while (s--) {
            x = mul(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    };
    if (!miller_rabin(2) || !miller_rabin(3)) return false;
    
    auto lucas_pseudoprime = [&]() {
        auto normalize = [&](__int128 &x) {
            if (x < 0) x += ((-x / n) + 1) * n;
        };

        __int128 D = -3;
        for (;;) {
            D += D > 0 ? 2 : -2;
            D *= -1;

            int jacobi = 1;
            auto jacobi_symbol = [&](__int128 n) {
                auto a = D;
                normalize(a);

                while (a) {
                    while (!(a & 1)) {
                        a >>= 1;
                        if ((n & 7) == 3 || (n & 7) == 5) jacobi = -jacobi;
                    }
                    if ((a & 3) == 3 && (n & 3) == 3) jacobi = -jacobi;

                    swap(a, n);
                    a %= n;
                }
                return n == 1;
            };
            if (!jacobi_symbol(n)) return false;
            if (jacobi == -1) break;
        }

        string bits;
        auto temp = n + 1;
        while (temp) {
            bits += (temp & 1) ? '1' : '0';
            temp >>= 1;
        }
        bits.pop_back();
        reverse(bits.begin(), bits.end());

        auto div2mod = [&](__int128 x) -> unsigned long long {
            if (x & 1) x += n;
            normalize(x >>= 1);

            return x % n;
        };

        __int128 U = 1, V = 1;
        for (char b : bits) {
            auto U_2k = mul(U, V, n), V_2k = div2mod(mul(V, V, n) + D * mul(U, U, n));

            if (b == '0') {
                U = U_2k;
                V = V_2k;
            } else {
                U = div2mod(U_2k + V_2k);
                V = div2mod(D * U_2k + V_2k);
            }
        }

        return !U;
    };
    return lucas_pseudoprime();
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
