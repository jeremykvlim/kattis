#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return ((unsigned __int128) x * y) % mod;
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
struct MontgomeryModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, unsigned __int128, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;
    using J = typename conditional<is_same<T, unsigned int>::value, long long, typename conditional<is_same<T, unsigned long long>::value, __int128, void>::type>::type;

    T value;
    static pair<T, U> r;
    static bool prime_mod;
    static constexpr int bit_length = sizeof(T) * 8;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == (unsigned long long) 1e9 + 7 || mod() == (unsigned long long) 1e9 + 9 || mod() == (unsigned long long) 1e6 + 69 || mod() == 2524775926340780033 || isprime(mod());
        r = {mod(), - (U) mod() % mod()};
        while (mod() * r.first != 1) r.first *= (T) 2 - mod() * r.first;
    }

    constexpr MontgomeryModInt() : value() {}

    MontgomeryModInt(const J &x) {
        value = reduce((U) x * r.second);
    }

    static T reduce(const U &x) {
        T q = (U) x * r.first, v = (x >> bit_length) + mod() - (((U) q * mod()) >> bit_length);
        return v >= mod() ? v - mod() : v;
    }

    T operator()() const {
        return reduce((U) value);
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    constexpr static T mod() {
        return M::value;
    }

    inline auto & operator+=(const MontgomeryModInt &v) {
        if ((I) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const MontgomeryModInt &v) {
        if ((I) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename U>
    inline auto & operator+=(const U &v) {
        return *this += (MontgomeryModInt) v;
    }

    template <typename U>
    inline auto & operator-=(const U &v) {
        return *this -= (MontgomeryModInt) v;
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
        return (MontgomeryModInt) 0 - *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned int>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((unsigned long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned long long>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((__uint128_t) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<!is_integral<typename MontgomeryModInt<V>::T>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce(value * v.value);
        return *this;
    }

    auto & operator/=(const MontgomeryModInt &v) {
        return *this *= inv(v);
    }

    MontgomeryModInt inv(const MontgomeryModInt &v) {
        if (prime_mod) {
            MontgomeryModInt inv = 1, base = v;
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

        return (MontgomeryModInt) x;
    }
};

template <typename T>
bool operator==(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template <typename T, typename U>
bool operator==(const MontgomeryModInt<T> &lhs, U rhs) {
    return lhs == MontgomeryModInt<T>(rhs);
}

template <typename T, typename U>
bool operator==(U lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) == rhs;
}

template <typename T>
bool operator!=(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(const MontgomeryModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(U lhs, const MontgomeryModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator>(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return lhs() > rhs();
}

template <typename T>
bool operator<(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return lhs() < rhs();
}

template <typename T>
bool operator>=(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return lhs > rhs || lhs == rhs;
}

template <typename T>
bool operator<=(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return lhs < rhs || lhs == rhs;
}

template <typename T>
MontgomeryModInt<T> operator+(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator+(const MontgomeryModInt<T> &lhs, U rhs) {
    return MontgomeryModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator+(U lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) += rhs;
}

template <typename T>
MontgomeryModInt<T> operator-(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator-(const MontgomeryModInt<T> &lhs, U rhs) {
    return MontgomeryModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator-(U lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) -= rhs;
}

template <typename T>
MontgomeryModInt<T> operator*(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator*(const MontgomeryModInt<T> &lhs, U rhs) {
    return MontgomeryModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator*(U lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) *= rhs;
}

template <typename T>
MontgomeryModInt<T> operator/(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator/(const MontgomeryModInt<T> &lhs, U rhs) {
    return MontgomeryModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
MontgomeryModInt<T> operator/(U lhs, const MontgomeryModInt<T> &rhs) {
    return MontgomeryModInt<T>(lhs) /= rhs;
}

template <typename U, typename T>
U & operator<<(U &stream, const MontgomeryModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, MontgomeryModInt<T> &v) {
    typename common_type<typename MontgomeryModInt<T>::T, long long>::type x;
    stream >> x;
    v = MontgomeryModInt<T>(x);
    return stream;
}

template <typename M>
pair<typename MontgomeryModInt<M>::T, typename MontgomeryModInt<M>::U> MontgomeryModInt<M>::r;

template <typename M>
bool MontgomeryModInt<M>::prime_mod;

constexpr unsigned int MODULO = 1e4 + 7;
using modint = MontgomeryModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int n, c;
    cin >> n >> c;

    vector<modint> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];

    modint total = 1, rgb = 1, bw = 1;
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (!b[i]) {
            count++;
            rgb *= a[i];
            continue;
        }

        a[i] /= b[i];
        total *= a[i] + 1;
        bw *= b[i];
    }

    vector<modint> dp(c);
    dp[0] = 1;
    for (int i = 1; i <= n; i++)
        if (b[i])
            for (int j = c - 1; j; j--) dp[j] += dp[j - 1] * a[i];

    int q;
    cin >> q;

    while (q--) {
        int p, ap, bp;
        cin >> p >> ap >> bp;

        if (b[p]) {
            bw /= b[p];
            total /= a[p] + 1;
            for (int j = 1; j < c; j++) dp[j] -= dp[j - 1] * a[p];
        } else {
            count--;
            rgb /= a[p];
        }

        a[p] = ap;
        b[p] = bp;

        if (b[p]) {
            a[p] /= bp;
            bw *= b[p];
            total *= a[p] + 1;
            for (int j = c - 1; j; j--) dp[j] += dp[j - 1] * a[p];
        } else {
            count++;
            rgb *= a[p];
        }

        if (count > c) {
            cout << "0\n";
            continue;
        }

        auto purchases = total;
        for (int j = 0; j < c - count; j++) purchases -= dp[j];
        cout << purchases * bw * rgb << "\n";
    }
}
