#include <bits/stdc++.h>
using namespace std;

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

bool isprime(long long n) {
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

template <typename M>
struct BarrettModInt {
    using I = unsigned int;
    using J = unsigned long long;
    using K = unsigned __int128;

    I value;
    static bool prime_mod;
    static J inv_mod;
    static constexpr int bit_length = sizeof(J) * 8;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == 1e9 + 7 || mod() == 1e9 + 9 || mod() == 1e6 + 69 || isprime(mod());
        inv_mod = (J) -1 / mod() + 1;
    }

    constexpr BarrettModInt() : value() {}

    template <typename V>
    BarrettModInt(const V &x) {
        value = normalize(x);
    }

    template <typename V>
    static I normalize(const V &x) {
        I v = x;
        if (!(-mod() <= x && x < mod())) v = (I) (x % mod());
        return v < 0 ? v + mod() : v;
    }

    static I reduce(J v) {
        v -= (J) (((K) v * inv_mod) >> bit_length) * mod();
        return v >= mod() ? v + mod() : v;
    }

    const I & operator()() const {
        return value;
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    constexpr static I mod() {
        return M::value;
    }

    inline auto & operator+=(const BarrettModInt &v) {
        if ((int) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const BarrettModInt &v) {
        if ((int) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename V>
    inline auto & operator+=(const V &v) {
        return *this += BarrettModInt(v);
    }

    template <typename V>
    inline auto & operator-=(const V &v) {
        return *this -= BarrettModInt(v);
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
        return (BarrettModInt) 0 - *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename BarrettModInt<V>::I, unsigned int>::value, BarrettModInt>::type & operator*=(const BarrettModInt &v) {
        value = reduce((J) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<!is_integral<typename BarrettModInt<V>::I>::value, BarrettModInt>::type & operator*=(const BarrettModInt &v) {
        value = reduce(value * v.value);
        return *this;
    }

    auto & operator/=(const BarrettModInt &v) {
        return *this *= inv(v);
    }

    BarrettModInt inv(const BarrettModInt &v) {
        if (prime_mod) {
            BarrettModInt inv = 1, base = v;
            I n = mod() - 2;
            while (n) {
                if (n & 1) inv *= base;
                base *= base;
                n >>= 1;
            }
            return inv;
        }

        I x = 0, y = 1, a = v.value, m = mod();
        while (a) {
            I t = m / a;
            m -= t * a;
            swap(a, m);
            x -= t * y;
            swap(x, y);
        }

        return (BarrettModInt) x;
    }
};

template <typename T>
bool operator==(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template <typename T, typename U>
bool operator==(const BarrettModInt<T> &lhs, U rhs) {
    return lhs == BarrettModInt<T>(rhs);
}

template <typename T, typename U>
bool operator==(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) == rhs;
}

template <typename T>
bool operator!=(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(const BarrettModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(U lhs, const BarrettModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator>(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return lhs.value > rhs.value;
}

template <typename T>
bool operator<(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template <typename T>
BarrettModInt<T> operator+(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator+(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator+(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) += rhs;
}

template <typename T>
BarrettModInt<T> operator-(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator-(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator-(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) -= rhs;
}

template <typename T>
BarrettModInt<T> operator*(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator*(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator*(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) *= rhs;
}

template <typename T>
BarrettModInt<T> operator/(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator/(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator/(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) /= rhs;
}

template <typename U, typename T>
U & operator<<(U &stream, const BarrettModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, BarrettModInt<T> &v) {
    typename common_type<typename BarrettModInt<T>::I, long long>::type x;
    stream >> x;
    v.value = BarrettModInt<T>::normalize(x);
    return stream;
}

template <typename M>
bool BarrettModInt<M>::prime_mod;

template <typename M>
unsigned long long BarrettModInt<M>::inv_mod;

constexpr unsigned int MODULO = 1e4;
using modint = BarrettModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    modint a;
    long long b;
    char op;
    while (cin >> a >> op >> b) {
        if (op == '^') cout << pow((long long) a.value, b, (long long) MODULO) << "\n";
        else cout << (op == '+' ? a + b : a * b) << "\n";
    }
}
