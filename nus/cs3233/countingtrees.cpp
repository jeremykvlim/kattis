#include <bits/stdc++.h>
using namespace std;

template<typename T>
T inverse(T a, T mod) {
    T u = 0, v = 1;
    while (a) {
        T t = mod / a;
        mod -= t * a;
        swap(a, mod);
        u -= t * v;
        swap(u, v);
    }

    return u;
}

template <typename T>
T mul(T x, T y, T mod) {
    long long px = x, py = y, pmod = mod;
    auto product = px * py - pmod * (long long) (1.L / pmod * px * py);
    return product + pmod * (product < 0) - pmod * (product >= pmod);
}

template <typename T>
T pow(T base, T exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

constexpr int MODULO1 = 3, MODULO2 = 11, MODULO3 = 101, MODULO4 = 3000301;
const bool PRIME_MOD = true;

template<typename M>
struct ModInt {
    using T = typename decay<decltype(M::value)>::type;
    T value;

    constexpr ModInt() : value() {}

    template<typename U>
    ModInt(const U &x) {
        value = normalize(x);
    }

    template<typename U>
    static T normalize(const U &x) {
        T v = x;
        if (!(-mod() <= x && x < mod())) v = (T) (x % mod());
        if (v < 0) v += mod();
        return v;
    }

    const T & operator()() const {
        return value;
    }

    template<typename U>
    explicit operator U() const {
        return (U) value;
    }

    constexpr static T mod() {
        return M::value;
    }

    inline auto & operator+=(const ModInt &v) {
        if ((value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const ModInt &v) {
        if ((value -= v.value) < 0) value += mod();
        return *this;
    }

    template<typename U>
    inline auto & operator+=(const U &v) {
        return *this += ModInt(v);
    }

    template<typename U>
    inline auto & operator-=(const U &v) {
        return *this -= ModInt(v);
    }

    auto & operator++() {
        return *this += 1;
    }

    auto & operator--() {
        return *this -= 1;
    }

    auto operator++(int) {
        ModInt v(*this);
        *this += 1;
        return v;
    }

    auto operator--(int) {
        ModInt v(*this);
        *this -= 1;
        return v;
    }

    auto operator-() const {
        return ModInt(-value);
    }

    template<typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, int>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize((long long) value * (long long) v.value);
        return *this;
    }

    template<typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, long long>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(value * v.value - (long long) ((long double) value * v.value / mod()) * mod());
        return *this;
    }

    template<typename U = M>
    typename enable_if<!is_integral<typename ModInt<U>::T>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(value * v.value);
        return *this;
    }

    auto & operator/=(const ModInt &v) {
        if (PRIME_MOD) return *this *= ModInt(pow(v.value, mod() - 2, mod()));
        return *this *= ModInt(inverse(v.value, mod()));
    }
};

template<typename T>
bool operator==(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template<typename T, typename U>
bool operator==(const ModInt<T> &lhs, U rhs) {
    return lhs == ModInt<T>(rhs);
}

template<typename T, typename U>
bool operator==(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) == rhs;
}

template<typename T>
bool operator!=(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator!=(const ModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator!=(U lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator<(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template<typename T>
ModInt<T> operator+(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template<typename T, typename U>
ModInt<T> operator+(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) += rhs;
}

template<typename T, typename U>
ModInt<T> operator+(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template<typename T>
ModInt<T> operator-(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template<typename T, typename U>
ModInt<T> operator-(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template<typename T, typename U>
ModInt<T> operator-(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template<typename T>
ModInt<T> operator*(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template<typename T, typename U>
ModInt<T> operator*(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template<typename T, typename U>
ModInt<T> operator*(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template<typename T>
ModInt<T> operator/(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template<typename T, typename U>
ModInt<T> operator/(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template<typename T, typename U>
ModInt<T> operator/(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename U, typename T>
U & operator<<(U &stream, const ModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, ModInt<T> &v) {
    typename common_type<typename ModInt<T>::T, long long>::type x;
    stream >> x;
    v.value = ModInt<T>::normalize(x);
    return stream;
}

using modint1 = ModInt<integral_constant<decay<decltype(MODULO1)>::type, MODULO1>>;
using modint2 = ModInt<integral_constant<decay<decltype(MODULO2)>::type, MODULO2>>;
using modint3 = ModInt<integral_constant<decay<decltype(MODULO3)>::type, MODULO3>>;
using modint4 = ModInt<integral_constant<decay<decltype(MODULO4)>::type, MODULO4>>;

template<typename T>
T C(long long n, long long k, int p, vector<T> &fact, vector<T> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return C(n / p, k / p, p, fact, fact_inv) * C(n % p, k % p, p, fact, fact_inv);
    return fact[n] * fact_inv[k] * fact_inv[n - k];
}

template<typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

long long crt(long long a, long long m, long long b, long long n) {
    auto g = __gcd(m, n);
    if ((b - a) % g) return -1;

    if (n > m) {
        swap(a, b);
        swap(m, n);
    }

    a %= m;
    b %= n;
    auto mod = m / g * n;
    auto [x, y] = bezout(m / g, n / g);
    auto X = a * (n / g) % mod * y % mod + b * (m / g) % mod * x % mod;
    if (X < 0) X += mod;
    return X;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<modint1> fact1(MODULO1 + 1, 1), fact_inv1(MODULO1 + 1, 1);
    vector<modint2> fact2(MODULO2 + 1, 1), fact_inv2(MODULO2 + 1, 1);
    vector<modint3> fact3(MODULO3 + 1, 1), fact_inv3(MODULO3 + 1, 1);
    vector<modint4> fact4(MODULO4 + 1, 1), fact_inv4(MODULO4 + 1, 1);

    auto prepare = [&](auto &fact, auto &fact_inv, int mod) {
        auto inv = fact;

        for (int i = 1; i <= mod; i++) {
            if (i > 1) inv[i] = (mod - mod / i) * inv[mod % i];
            fact[i] = i * fact[i - 1];
            fact_inv[i] = inv[i] * fact_inv[i - 1];
        }
    };
    prepare(fact1, fact_inv1, MODULO1);
    prepare(fact2, fact_inv2, MODULO2);
    prepare(fact3, fact_inv3, MODULO3);
    prepare(fact4, fact_inv4, MODULO4);

    while (t--) {
        long long n, k, c;
        cin >> n >> k >> c;

        auto c_good = 0LL, mod_product = 1LL;
        auto count = [&](long long n, long long c, auto &fact, auto &fact_inv, int mod) {
            auto choices = 2 * C(n - 1, c - 1, mod, fact, fact_inv);
            c_good = crt(c_good, mod_product, choices.value, mod);
            mod_product *= mod;
        };
        count(n, c, fact1, fact_inv1, MODULO1);
        count(n, c, fact2, fact_inv2, MODULO2);
        count(n, c, fact3, fact_inv3, MODULO3);
        count(n, c, fact4, fact_inv4, MODULO4);

        cout << c_good << "\n";
    }
}
