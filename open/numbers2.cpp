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
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    inline auto & operator+=(const V &v) {
        return *this += BarrettModInt(v);
    }

    template <typename V>
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    friend bool operator==(const BarrettModInt &lhs, V rhs) {
        return lhs == BarrettModInt(rhs);
    }

    template <typename V>
    requires is_integral_v<V>
    friend bool operator==(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) == rhs;
    }

    friend bool operator!=(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires is_integral_v<V>
    friend bool operator!=(const BarrettModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    friend BarrettModInt operator+(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) += rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend BarrettModInt operator+(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) += rhs;
    }

    friend BarrettModInt operator-(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend BarrettModInt operator-(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend BarrettModInt operator-(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) -= rhs;
    }

    friend BarrettModInt operator*(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend BarrettModInt operator*(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend BarrettModInt operator*(V lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) *= rhs;
    }

    friend BarrettModInt operator/(const BarrettModInt &lhs, const BarrettModInt &rhs) {
        return BarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend BarrettModInt operator/(const BarrettModInt &lhs, V rhs) {
        return BarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
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

constexpr int MOD = 5318008;
using modint = BarrettModInt<MOD>;

template <typename T>
T kitamasa(const vector<T> &c, const vector<T> &a, long long k) {
    int n = a.size();

    auto mul = [&](const vector<T> &x, const vector<T> &y) {
        vector<T> z(2 * n + 1, 0);
        for (int i = 0; i <= n; i++)
            if (x[i])
                for (int j = 0; j <= n; j++) z[i + j] += x[i] * y[j];
    
        for (int i = 2 * n; i > n; i--)
            if (z[i])
                for (int j = 0; j < n; j++) z[i - j - 1] += z[i] * c[j];

        z.resize(n + 1);
        return z;
    };

    vector<T> base(n + 1, 0);
    base[1] = 1;
    auto pow = [&](vector<T> base, long long exponent) {
        vector<T> value(n + 1);
        value[0] = 1;
        while (exponent) {
            if (exponent & 1) value = mul(value, base);
            base = mul(base, base);
            exponent >>= 1;
        }
        return value;
    };
    auto value = pow(base, k + 1);

    T kth = 0;
    for (int i = 0; i < n; i++) kth += value[i + 1] * a[i];
    return kth;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int t;
    cin >> t;

    vector<modint> c{6, -4}, a{1, 3};
    for (int x = 1; x <= t; x++) {
        int n;
        cin >> n;
        cout << "Case #" << x << ": " << setw(3) << setfill('0') << 2 * kitamasa(c, a, n) - 1 << "\n";
    }
}
