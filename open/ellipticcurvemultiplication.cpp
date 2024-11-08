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
struct ModInt {
    using T = typename decay<decltype(M::value)>::type;

    T value;
    static bool prime_mod;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == 1e9 + 7 || mod() == 1e9 + 9 || mod() == 1e6 + 69 || mod() == 2524775926340780033 || isprime(mod());
    }

    constexpr ModInt() : value() {}

    template <typename U>
    ModInt(const U &x) {
        value = normalize(x);
    }

    template <typename U>
    static T normalize(const U &x) {
        T v = x;
        if (!(-mod() <= x && x < mod())) v = (T) (x % mod());
        if (v < 0) v += mod();
        return v;
    }

    const T & operator()() const {
        return value;
    }

    template <typename U>
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

    template <typename U>
    inline auto & operator+=(const U &v) {
        return *this += ModInt(v);
    }

    template <typename U>
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

    template <typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, int>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = normalize((long long) value * (long long) v.value);
        return *this;
    }

    template <typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, long long>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = normalize(mul(value, v.value, mod()));
        return *this;
    }

    template <typename U = M>
    typename enable_if<!is_integral<typename ModInt<U>::T>::value, ModInt>::type & operator*=(const ModInt &v) {
        value = normalize(value * v.value);
        return *this;
    }

    auto & operator/=(const ModInt &v) {
        return *this *= inv(v);
    }

    ModInt inv(const ModInt &v) {
        if (prime_mod) {
            ModInt inv = 1, base = v;
            T n = mod() - 2;
            while (n) {
                if (n & 1) inv *= base;
                base *= base;
                n >>= 1;
            }
            return inv;
        }

        T x = 0, y = 1, a = v.value, m = mod();
        while (a) {
            T t = m / a;
            m -= t * a;
            swap(a, m);
            x -= t * y;
            swap(x, y);
        }

        return (ModInt) x;
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
    v.value = ModInt<T>::normalize(x);
    return stream;
}

template <typename M>
bool ModInt<M>::prime_mod;

template <typename T>
struct MODULO {
    static T value;
};

template <typename T>
T MODULO<T>::value;

auto &m = MODULO<long long>::value;
using modint = ModInt<MODULO<long long>>;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

template <typename T, typename U>
Point<T> add(Point<T> p, Point<T> q, U a) {
    if (p == Point<T>{-1, -1}) return q;
    if (q == Point<T>{-1, -1}) return p;

    if (p.x == q.x && (p.y == -q.y || p.y == q.y && p.y == 0)) return {-1, -1};

    T lambda = p == q ? (3 * p.x * p.x + a) / (2 * p.y) : (q.y - p.y) / (q.x - p.x), xr = lambda * lambda - p.x - q.x, yr = lambda * (p.x - xr) - p.y;
    return {xr, yr};
}

template <typename T, typename U>
Point<T> multiply(Point<T> base, U exponent, U a) {
    Point<T> p{-1, -1};
    while (exponent) {
        if (exponent & 1) p = add(p, base, a);
        base = add(base, base, a);
        exponent >>= 1;
    }
    return p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b, n, x2, y2;
    cin >> m >> a >> b >> n >> x2 >> y2;

    if (!n || !~x2 && !~y2) {
        cout << "-1 -1";
        exit(0);
    }

    modint::init();

    auto np = multiply(Point<modint>{x2, y2}, n, a);
    if (np == Point<modint>{-1, -1}) cout << "-1 -1";
    else cout << np.x << " " << np.y;
}
