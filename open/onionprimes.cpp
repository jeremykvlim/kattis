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
        if (exponent & 1) value = mul(base, value, mod);
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
        int s = countr_zero(n - 1);
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> onion_primes(11);
    for (int b = 2; b <= 10; b++) {
        int pow = 0;
        for (auto val = 1LL; val <= INT_MAX; val *= b) pow++;

        vector<long long> pb(pow + 1, 1);
        for (int i = 1; i <= pow; i++) pb[i] = pb[i - 1] * b;

        vector<pair<int, int>> curr;
        for (int p = 2; p < b; p == 2 ? p++ : p += 2)
            if (isprime(p)) {
                curr.emplace_back(p, 1);
                onion_primes[b].emplace_back(p);
            }

        for (int p = b; p < pb[2]; p++)
            if (isprime(p)) {
                curr.emplace_back(p, 2);
                onion_primes[b].emplace_back(p);
            }

        while (!curr.empty()) {
            vector<pair<int, int>> temp;
            for (auto [p, len] : curr) {
                if (pow < len + 2) continue;

                for (int d_first = 1; d_first < b; d_first++) {
                    auto v = pb[len + 1] * d_first + (long long) p * b;
                    for (int d_last = 0; d_last < b; d_last++) {
                        auto q = v + d_last;
                        if (q > INT_MAX) continue;

                        if (isprime(q)) {
                            temp.emplace_back(q, len + 2);
                            onion_primes[b].emplace_back(q);
                        }
                    }
                }
            }
            curr = temp;
        }

        sort(onion_primes[b].begin(), onion_primes[b].end());
        onion_primes[b].erase(unique(onion_primes[b].begin(), onion_primes[b].end()), onion_primes[b].end());
    }

    int t;
    cin >> t;

    while (t--) {
        int b, A, B;
        cin >> b >> A >> B;

        auto l = lower_bound(onion_primes[b].begin(), onion_primes[b].end(), A), r = upper_bound(onion_primes[b].begin(), onion_primes[b].end(), B);
        cout << r - l << "\n";
    }
}
