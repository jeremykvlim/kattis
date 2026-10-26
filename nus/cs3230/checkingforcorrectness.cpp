#include <bits/stdc++.h>
using namespace std;

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

template <auto M>
struct BarrettModInt {
    using H = long long;
    using I = unsigned int;
    using J = unsigned long long;
    using K = unsigned __int128;

    I value;

    constexpr static H mod() {
        return M;
    }

    static constexpr J inv_mod = (J) -1 / mod();
    static constexpr int bit_length = sizeof(J) * 8;
    static inline bool prime_mod = isprime(mod());

    constexpr BarrettModInt() : value() {}

    template <typename V>
    requires numeric_limits<V>::is_integer
    BarrettModInt(const V &x) {
        value = normalize(x);
    }

    template <typename V>
    static I normalize(const V &x) {
        V v = x;
        if (!(-mod() <= x && x < mod())) v = x % mod();
        return v < 0 ? v + mod() : v;
    }

    static I reduce(J v) {
        H r = (H) (v - (((K) v * inv_mod) >> bit_length) * mod());
        return r >= mod() ? r - mod() : r;
    }

    const I & operator()() const {
        return value;
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    inline auto & operator+=(const BarrettModInt &v) {
        H t = (H) value + v.value;
        value = t >= mod() ? t - mod() : t;
        return *this;
    }

    inline auto & operator-=(const BarrettModInt &v) {
        H t = (H) value - v.value;
        value = t < 0 ? t + mod() : t;
        return *this;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator+=(const V &v) {
        return *this += BarrettModInt(v);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
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
        auto t = *this;
        *this += 1;
        return t;
    }

    auto operator--(int) {
        auto t = *this;
        *this -= 1;
        return t;
    }

    auto operator-() const {
        return (BarrettModInt) 0 - *this;
    }

    auto & operator*=(const BarrettModInt &v) {
        value = reduce((J) value * v.value);
        return *this;
    }

    auto & operator/=(const BarrettModInt &v) {
        return *this *= inv(v);
    }

    friend bool operator==(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return lhs.value == rhs.value;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(const BarrettModInt &lhs, V rhs) {
        return lhs == BarrettModInt(rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) == rhs;
    }

    friend bool operator!=(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(const BarrettModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(V lhs, const BarrettModInt &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator>(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return lhs.value > rhs.value;
    }

    friend bool operator<(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return lhs.value < rhs.value;
    }

    friend BarrettModInt operator+(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator+(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator+(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) += rhs;
    }

    friend BarrettModInt operator-(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator-(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator-(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) -= rhs;
    }

    friend BarrettModInt operator*(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator*(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator*(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) *= rhs;
    }

    friend BarrettModInt operator/(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator/(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend BarrettModInt operator/(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) /= rhs;
    }

    template <typename S>
    friend S & operator<<(S &stream, const BarrettModInt &v) {
        return stream << v();
    }

    template <typename S>
    friend S & operator>>(S &stream, BarrettModInt &v) {
        H x;
        stream >> x;
        v = BarrettModInt(x);
        return stream;
    }

    static BarrettModInt pow(BarrettModInt base, J exponent) {
        BarrettModInt v = 1;
        while (exponent) {
            if (exponent & 1) v *= base;
            base *= base;
            exponent >>= 1;
        }
        return v;
    }

    static BarrettModInt inv(const BarrettModInt &v) {
        if (prime_mod) return pow(v, mod() - 2);

        H x = 0, y = 1, a = v(), m = mod();
        while (a) {
            H t = m / a;
            m -= t * a;
            swap(a, m);
            x -= t * y;
            swap(x, y);
        }

        return (BarrettModInt) x;
    }
};

constexpr int MOD = 1e4;
using modint = BarrettModInt<MOD>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint a;
    long long b;
    char op;
    while (cin >> a >> op >> b) {
        if (op == '^') cout << modint::pow(a, b) << "\n";
        else cout << (op == '+' ? a + b : a * b) << "\n";
    }
}