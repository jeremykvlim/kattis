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

template <typename T>
T tonelli_shanks(T n, T p) {
    if (p == 2) return n;
    if (p % 4 == 3) return pow(n, p / 4 + 1, p);

    T q = p - 1;
    int s = 0;
    for (; !(q & 1); q >>= 1, s++);

    auto legendre = [&](T a, T p) -> int {
        if (!a) return 0;
        return pow(a, (p - 1) / 2, p) != p - 1 ? 1 : -1;
    };

    T z = 2;
    while (legendre(z, p) != -1) z++;

    int m = s;
    T c = pow(z, q, p), t = pow(n, q, p), r = pow(n, (q + 1) / 2, p);
    while (t != 1) {
        T temp = t;

        int i = 0;
        for (; temp != 1 && i < m; i++) temp = mul(temp, temp, p);

        T b = c;
        for (int j = 0; j < m - i - 1; j++) b = mul(b, b, p);
        m = i;
        c = mul(b, b, p);
        t = mul(t, c, p);
        r = mul(r, b, p);
    }
    return r;
}

template <typename T>
pair<T, T> cornacchia(T d, T m) {
    T r0 = m, r1 = tonelli_shanks(m - d % m, m);
    for (; r1 > sqrt(m); r0 = exchange(r1, r0 % r1));

    T x = r1;
    if (!d) return {x, 0};

    T y = sqrt((m - d * x * x) / d);
    return {x, y};
}

bool isprime(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 5 || n == 11) return true;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    auto inv = n, one = -n % n;
    while (n * inv != 1) inv *= 2 - n * inv;

    auto add = [&](auto x, auto y) {
        return x >= n - y ? x - (n - y) : x + y;
    };

    auto sub = [&](auto x, auto y) {
        return x >= y ? x - y : n - (y - x);
    };

    auto mul = [&](auto x, auto y) -> unsigned long long {
        auto z = (unsigned __int128) x * y;
        unsigned long long q = z * inv;
        auto v = (z >> 64) + n - (((unsigned __int128) q * n) >> 64);
        return v >= n ? v - n : v;
    };

    auto pow = [&](auto base, auto exponent) {
        auto value = one;
        while (exponent) {
            if (exponent & 1) value = mul(value, base);
            base = mul(base, base);
            exponent >>= 1;
        }
        return value;
    };

    auto two = add(one, one);
    auto miller_rabin = [&](auto a) {
        int s = countr_zero(n - 1);
        auto x = pow(a, (n - 1) >> s);
        if (x == one || x == n - one) return true;

        while (--s) {
            x = mul(x, x);
            if (x == n - one) return true;
        }
        return false;
    };
    if (!miller_rabin(two) || !miller_rabin(add(two, one))) return false;

    auto P = 3ULL;
    for (;; P++) {
        auto D = P * P - 4;
        auto jacobi_symbol = [&](auto n) {
            int j = 1;
            auto a = D;
            while (a) {
                for (; !(a & 1); a >>= 1)
                    if ((n & 7) == 3 || (n & 7) == 5) j = -j;
                if ((a & 3) == 3 && (n & 3) == 3) j = -j;

                swap(a, n);
                a %= n;
            }
            return n == 1 ? j : 0;
        };
        int j = jacobi_symbol(n);
        if (j == -1) break;
        if (!j && D % n) return false;
    }
    P = ((unsigned __int128) P * one % n);

    int s = countr_zero(n + 1);
    auto k = (n + 1) >> s, V_k = P, V_kp1 = sub(mul(P, P), two);
    for (int i = bit_width(k) - 2; i >= 0; i--) {
        auto V_2kp1 = sub(mul(V_k, V_kp1), P);

        if ((k >> i) & 1) {
            V_k = V_2kp1;
            V_kp1 = sub(mul(V_kp1, V_kp1), two);
        } else {
            V_kp1 = V_2kp1;
            V_k = sub(mul(V_k, V_k), two);
        }
    }
    if ((V_k == two && V_kp1 == P) || (V_k == sub(0, two) && V_kp1 == sub(0, P))) return true;

    while (--s) {
        if (!V_k) return true;
        V_k = sub(mul(V_k, V_k), two);
    }
    return false;
}

unsigned long long pollard_brent(unsigned long long n) {
    if (!(n & 1)) return 2;

    auto inv = n, one = -n % n;
    while (n * inv != 1) inv *= 2 - n * inv;

    auto add = [&](auto x, auto y) {
        return x >= n - y ? x - (n - y) : x + y;
    };

    auto mul = [&](auto x, auto y) -> unsigned long long {
        auto z = (unsigned __int128) x * y;
        unsigned long long q = z * inv;
        auto v = (z >> 64) + n - (((unsigned __int128) q * n) >> 64);
        return v >= n ? v - n : v;
    };

    static mt19937_64 rng(random_device{}());
    for (;;) {
        auto x = add(one, one), y = x, z = one, xs = x, d = 1ULL, c = rng() % (n - 1) + 1;

        auto f = [&](auto x) {
            return add(mul(x, x), c);
        };

        for (int i = 1; d == 1; i <<= 1, y = x) {
            for (int _ = 0; _ < i - 1; _++) x = f(x);
            for (int j = 0; j < i && d == 1; j += 300) {
                xs = x;
                for (int k = 0; k < min(300, i - j); k++) {
                    x = f(x);
                    z = mul(z, max(x, y) - min(x, y));
                }
                d = __gcd(z, n);
            }
        }

        if (d == n) d = 1;
        while (d == 1) {
            xs = f(xs);
            d = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (d != n) return d;
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

        T d = pollard_brent(m);
        self(self, d);
        self(self, m / d);
    };
    dfs(dfs, n);

    return {pfs.begin(), pfs.end()};
}

template <typename T>
struct GaussianInteger {
    T a, b;

    GaussianInteger(T a = 0, T b = 0) : a(a), b(b) {}

    GaussianInteger operator-() const {
        return {-a, -b};
    }

    GaussianInteger operator~() const {
        return {-b, a};
    }

    GaussianInteger operator+(const GaussianInteger &z) const {
        return {a + z.a, b + z.b};
    }

    GaussianInteger operator-(const GaussianInteger &z) const {
        return {a - z.a, b - z.b};
    }

    GaussianInteger operator*(const GaussianInteger &z) const {
        return {a * z.a - b * z.b, a * z.b + b * z.a};
    }

    GaussianInteger operator/(const GaussianInteger &z) const {
        T N = z.norm(), x = a * z.a + b * z.b, y = b * z.a - a * z.b;
        auto round_div = [N](T a) {
            T q = a / N, r = a % N;
            if (r >= (N + 1) / 2) q++;
            if (r <= -(N + 1) / 2) q--;
            return q;
        };
        return {round_div(x), round_div(y)};
    }

    GaussianInteger operator%(const GaussianInteger &z) const {
        return *this - *this / z * z;
    }

    GaussianInteger & operator+=(const GaussianInteger &z) {
        return *this = *this + z;
    }

    GaussianInteger & operator-=(const GaussianInteger &z) {
        return *this = *this - z;
    }

    GaussianInteger & operator*=(const GaussianInteger &z) {
        return *this = *this * z;
    }

    GaussianInteger & operator/=(const GaussianInteger &z) {
        return *this = *this / z;
    }

    GaussianInteger & operator%=(const GaussianInteger &z) {
        return *this = *this % z;
    }

    bool operator==(const GaussianInteger &) const = default;

    GaussianInteger conj() const {
        return {a, -b};
    }

    T norm() const {
        return a * a + b * b;
    }

    template <typename U>
    static GaussianInteger pow(GaussianInteger base, U exponent) {
        GaussianInteger value = 1;
        while (exponent) {
            if (exponent & 1) value *= base;
            base *= base;
            exponent >>= 1;
        }
        return value;
    }
};

template <typename T>
vector<pair<T, T>> sum_of_two_squares(T n) {
    if (!n) return {{0, 0}};

    auto pfs = factorize(n);
    vector<GaussianInteger<T>> pairs{{1, 0}};
    for (auto [pf, exponent] : pfs) {
        if (pf % 4 == 3 && exponent & 1) return {};

        vector<GaussianInteger<T>> pps;
        if (pf % 4 == 3) {
            T pp = 1;
            for (int i = 0; i < exponent / 2; i++) pp *= pf;
            pps.emplace_back(pp);
        } else if (pf == 2) pps.emplace_back(GaussianInteger<T>::pow({1, 1}, exponent));
        else {
            auto [a, b] = cornacchia((T) 1, pf);
            GaussianInteger<T> z{a, b};
            vector<GaussianInteger<T>> pZ(exponent + 1, {1, 0});
            for (int i = 1; i <= exponent; i++) pZ[i] = pZ[i - 1] * z;

            pps.resize(exponent + 1);
            for (int i = 0; i <= exponent; i++) pps[i] = pZ[i] * pZ[exponent - i].conj();
        }

        vector<GaussianInteger<T>> temp;
        for (auto z1 : pairs)
            for (auto z2 : pps) temp.emplace_back(z1 * z2);
        pairs = temp;
    }

    vector<pair<T, T>> sums;
    unordered_set<T> seen;
    for (auto z : pairs) {
        while (z.a <= 0 || z.b < 0) z = ~z;
        if (seen.count(z.a) || seen.count(z.b)) continue;

        seen.emplace(z.a);
        seen.emplace(z.b);
        sums.emplace_back(z.a, z.b);
    }
    return sums;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int count = 1;
    for (int c = 1; c <= n; c++) count += sum_of_two_squares((long long) c * (c + 1)).size();
    cout << count;
}
