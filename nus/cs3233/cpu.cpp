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

    auto adjust = [&](__int128 &x) {
        if (x < 0) x += ((-x / n) + 1) * n;
    };

    auto lucas_pseudoprime = [&]() {
        __int128 D = -3;
        for (;;) {
            D += D > 0 ? 2 : -2;
            D *= -1;

            int jacobi = 1;
            auto jacobi_symbol = [&](__int128 n) {
                auto a = D;
                adjust(a);

                while (a) {
                    while (!(a & 1)) {
                        a >>= 1;
                        if ((n & 7) == 3 || (n & 7) == 5) jacobi = -jacobi;
                    }
                    if (((a & 3) == 3) && ((n & 3) == 3)) jacobi = -jacobi;

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
            x >>= 1;

            adjust(x);
            return x % n;
        };

        __int128 U = 1, V = 1;
        for (char b : bits) {
            auto U_2k = mul(U, V, n), V_2k = div2mod(mul(V, V, n) + D * mul(U, U, n));
            U = U_2k;
            V = V_2k;

            if (b == '1') {
                auto U_2k_plus_1 = div2mod(U + V), V_2k_plus_1 = div2mod(D * U + V);
                U = U_2k_plus_1;
                V = V_2k_plus_1;
            }
        }

        return !U;
    };
    return lucas_pseudoprime();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> zeisel;
    for (int a = 1LL; a < 1e5; a++)
        for (int b = 2 - a; a * (a + b - 1) < 1e5; b++) {
            for (auto n = 0LL, p = 1LL, z = 1LL;; n++) {
                p = p * a + b;

                if (!isprime(p) || (long long) p * z > 2e9) break;

                z *= p;
                if (n > 1) zeisel.emplace_back(z);
            }
        }
    sort(zeisel.begin(), zeisel.end());

    int n;
    cin >> n;

    while (n--) {
        long long l, h;
        cin >> l >> h;

        auto lo = lower_bound(zeisel.begin(), zeisel.end(), l) - zeisel.begin(), hi = lower_bound(zeisel.begin(), zeisel.end(), h + 1) - zeisel.begin();
        cout << hi - lo << "\n";
    }
}
