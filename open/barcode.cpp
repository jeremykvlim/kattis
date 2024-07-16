#include <bits/stdc++.h>
using namespace std;

template <typename T>
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

template <typename T>
struct MODULO {
    static T value;
};

template <typename T>
T MODULO<T>::value;

auto &m = MODULO<unsigned int>::value;
bool PRIME_MOD;

template <typename M>
struct ModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, __uint128_t, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;

    T value;
    static pair<T, U> r;
    static constexpr int bit_length = sizeof(T) * 8;

    static void init() {
        r = {mod(), - (U) mod() % mod()};
        while (mod() * r.first != 1) r.first *= 2ULL - mod() * r.first;
    }

    constexpr ModInt() : value() {}

    ModInt(const U &x) {
        value = reduce((U) x * r.second);
    }

    static T reduce(const U &x) {
        T q = (U) x * r.first, v = (x >> bit_length) + mod() - (((U) q * mod()) >> bit_length);
        if (v >= mod()) v -= mod();
        return v;
    }

    T operator()() const {
        auto v = reduce((U) value);
        return v;
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    constexpr static T mod() {
        return M::value;
    }

    inline auto & operator+=(const ModInt &v) {
        if ((I) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const ModInt &v) {
        if ((I) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename U>
    inline auto & operator+=(const U &v) {
        return *this += (ModInt) v;
    }

    template <typename U>
    inline auto & operator-=(const U &v) {
        return *this -= (ModInt) v;
    }

    auto & operator++() {
        return *this += 1;
    }

    auto & operator--() {
        return *this -= 1;
    }

    auto operator++(int) {
        return *this += 1;
    }

    auto operator--(int) {
        return *this -= 1;
    }

    auto operator-() const {
        return (ModInt) -value;
    }

    template <typename V = M>
    typename enable_if<is_same<typename ModInt<V>::T, int>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = reduce((long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename ModInt<V>::T, unsigned int>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = reduce((unsigned long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename ModInt<V>::T, long long>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = reduce((__int128) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename ModInt<V>::T, unsigned long long>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = reduce((__uint128_t) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<!is_integral<typename ModInt<V>::T>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = reduce(value * v.value);
        return *this;
    }

    auto & operator/=(const ModInt &v) {
        if (PRIME_MOD) return *this *= (ModInt) pow(v.value, mod() - 2, mod());
        return *this *= (ModInt) inverse(v.value, mod());
    }
};

template <typename T>
bool operator==(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template <typename T, typename U>
bool operator==(const ModInt<T> &lhs, U rhs) {
    return lhs == ModInt<T>(rhs);
}

template <typename T, typename U>
bool operator==(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) == rhs;
}

template <typename T>
bool operator!=(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(const ModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(U lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator>(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value > rhs.value;
}

template <typename T>
bool operator<(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template <typename T>
ModInt<T> operator+(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
ModInt<T> operator+(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
ModInt<T> operator+(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T>
ModInt<T> operator-(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
ModInt<T> operator-(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
ModInt<T> operator-(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T>
ModInt<T> operator*(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
ModInt<T> operator*(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
ModInt<T> operator*(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T>
ModInt<T> operator/(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
ModInt<T> operator/(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
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
    v.value = ModInt<T>::reduce(x);
    return stream;
}

template <typename M>
pair<typename ModInt<M>::T, typename ModInt<M>::U> ModInt<M>::r;
using modint = ModInt<MODULO<unsigned int>>;

template<typename T>
T C(long long n, long long k, int p, vector<T> &fact, vector<T> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return C(n / p, k / p, p, fact, fact_inv) * C(n % p, k % p, p, fact, fact_inv);
    return fact[n] * fact_inv[k] * fact_inv[n - k];
}

template <typename T>
pair<T, T> fib(long long n, T mod = 1) {
    if (!n) return {0, 1};
    else {
        auto [f1, f2] = fib(n >> 1, mod);

        auto fib1 = mul(f1, (2 * f2 - f1 + mod) % mod, mod);
        auto fib2 = (mul(f1, f1, mod) + mul(f2, f2, mod)) % mod;

        if (n & 1) return {fib2, (fib1 + fib2) % mod};
        else return {fib1, fib2};
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n >> m;
        PRIME_MOD = isprime(m);
        modint::init();

        vector<modint> fact(n + 1, 1), fact_inv(n / 2 + 1, 1);
        auto prepare = [&]() {
            vector<modint> inv(n / 2 + 1, 1);

            for (int i = 1; i <= n / 2; i++) {
                if (i > 1) inv[i] = (m - m / i) * inv[m % i];
                fact_inv[i] = inv[i] * fact_inv[i - 1];
            }

            for (int i = 1; i <= n; i++) fact[i] = i * fact[i - 1];
        };
        prepare();

        auto [fib1, fib2] = fib(n, m);
        modint count = fib1 + fib2;
        if (!(n & 1)) {
            count -= n / 2 + 1;
            count += C(n, n / 2, m, fact, fact_inv);
        }

        cout << count << "\n";
    }
}
