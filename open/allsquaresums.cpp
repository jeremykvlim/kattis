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

template <typename T>
T brent(T n) {
    if (!(n & 1)) return 2;

    mt19937_64 rng(random_device{}());
    for (;;) {
        T x = 2, y = 2, g = 1, q = 1, xs = 1, c = rng() % (n - 1) + 1;
        for (int i = 1; g == 1; i <<= 1, y = x) {
            for (int j = 1; j < i; j++) x = mul(x, x, n) + c;
            for (int j = 0; j < i && g == 1; j += 128) {
                xs = x;
                for (int k = 0; k < min(128, i - j); k++) {
                    x = mul(x, x, n) + c;
                    q = mul(q, max(x, y) - min(x, y), n);
                }
                g = __gcd(q, n);
            }
        }

        if (g == n) g = 1;
        while (g == 1) {
            xs = mul(xs, xs, n) + c;
            g = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (g != n) return isprime(g) ? g : brent(g);
    }
}

template <typename T>
vector<pair<T, int>> factorize(T n) {
    unordered_map<T, int> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs[m]++;
            return;
        }

        T pf = brent(m);
        pfs[pf]++;
        self(self, m / pf);
    };
    dfs(dfs, n);

    return {pfs.begin(), pfs.end()};
}

template <typename T>
T tonelli_shanks(T n, T p) {
    if (p == 2) return n;
    if (p % 4 == 3) return pow(n, (p + 1) / 4, p);

    T q = p - 1;
    int s = 0;
    while (!(q & 1)) {
        q >>= 1;
        s++;
    }

    auto legendre = [&](T a, T p) -> int {
        if (!a) return 0;
        return pow(a, (p - 1) / 2, p) != p - 1 ? 1 : -1;
    };

    T z = 2;
    while (legendre(z, p) != -1) z++;

    int m = s;
    T c = pow(z, q, p), t = pow(n, q, p), r = pow(n, (q + 1) / 2, p);
    while (t != 1) {
        auto temp = t;

        int i = 0;
        while (temp != 1 && i < m) {
            temp = mul(temp, temp, p);
            i++;
        }

        T b = pow(c, 1LL << (m - i - 1), p);
        m = i;
        c = mul(b, b, p);
        t = mul(t, mul(b, b, p), p);
        r = mul(r, b, p);
    }
    return r;
}

template <typename T>
pair<T, T> cornacchia(T d, T m) {
    T a = tonelli_shanks(m - d % m, m);

    T r0 = m, r1 = a;
    long long sqrt_m = sqrtl(m);
    while (r1 > sqrt_m) {
        auto r2 = r0 % r1;
        r0 = r1;
        r1 = r2;
    }

    T x = r1;
    if (!d) return {x, 0};

    T y = sqrtl((m - d * x * x) / d);
    return {x, y};
}

template <typename T>
vector<pair<T, T>> two_square_sums(T n) {
    if (!n) return {{0, 0}};

    auto pfs = factorize(n);

    auto gaussian_int_forms = [&](T p, int exponent) -> vector<complex<T>> {
        if (p % 4 == 3) {
            T value = 1;
            for (int i = 0; i < exponent / 2; i++) value *= p;
            return {{value}};
        }

        if (p == 2) {
            auto pow = [&](complex<T> base, auto exponent) {
                complex<T> value(1, 0);
                while (exponent) {
                    if (exponent & 1) value *= base;
                    base *= base;
                    exponent >>= 1;
                }
                return value;
            };
            return {pow({1, 1}, exponent)};
        }

        auto [a, b] = cornacchia(1LL, p);
        complex<T> Z(a, b);
        vector<complex<T>> pZ(exponent + 1, {1, 0}), forms(exponent + 1);
        for (int i = 1; i <= exponent; i++) pZ[i] = pZ[i - 1] * Z;
        for (int i = 0; i <= exponent; i++) forms[i] = pZ[i] * conj(pZ[exponent - i]);
        return forms;
    };

    vector<complex<T>> pairs{{1, 0}};
    for (auto [pf, pow] : pfs) {
        if (pf % 4 == 3 && pow & 1) return {};
        vector<complex<T>> temp, forms = gaussian_int_forms(pf, pow);
        for (auto Z1 : pairs)
            for (auto Z2 : forms) temp.emplace_back(Z1 * Z2);
        pairs = temp;
    }

    vector<pair<T, T>> sums;
    unordered_set<T> seen;
    for (auto &Z : pairs) {
        while (Z.real() <= 0 || Z.imag() < 0) Z = {-Z.imag(), Z.real()};
        auto a = Z.real(), b = Z.imag();
        if (seen.count(a) || seen.count(b)) continue;
        seen.emplace(a);
        seen.emplace(b);
        sums.emplace_back(a, b);
    }
    return sums;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    if (!n) {
        cout << "1\n0^2 + 0^2 = 0";
        exit(0);
    }

    unordered_set<string> ways;
    for (auto [x, y] : two_square_sums(n)) {
        ways.emplace(to_string(x) + "^2 + " + to_string(y) + "^2 = " + to_string(n));
        ways.emplace(to_string(y) + "^2 + " + to_string(x) + "^2 = " + to_string(n));

        auto pos_x_neg_y = [&]() {
            ways.emplace(to_string(x) + "^2 + (-" + to_string(y) + ")^2 = " + to_string(n));
            ways.emplace("(-" + to_string(y) + ")^2 + " + to_string(x) + "^2 = " + to_string(n));
        };

        auto pos_y_neg_x = [&]() {
            ways.emplace("(-" + to_string(x) + ")^2 + " + to_string(y) + "^2 = " + to_string(n));
            ways.emplace(to_string(y) + "^2 + (-" + to_string(x) + ")^2 = " + to_string(n));
        };

        if (!x) pos_x_neg_y();
        else if (!y) pos_y_neg_x();
        else {
            pos_x_neg_y();
            pos_y_neg_x();
            ways.emplace("(-" + to_string(x) + ")^2 + (-" + to_string(y) + ")^2 = " + to_string(n));
            ways.emplace("(-" + to_string(y) + ")^2 + (-" + to_string(x) + ")^2 = " + to_string(n));
        }
    }

    cout << ways.size() << "\n";
    for (auto w : ways) cout << w << "\n";
}
