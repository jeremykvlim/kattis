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

constexpr unsigned long long MODULO = 2524775926340780033;
const bool PRIME_MOD = MODULO == 998244353 || MODULO == 1e9 + 7 || MODULO == 1e9 + 9 || MODULO == 1e6 + 69 || MODULO == 2524775926340780033 || isprime(MODULO);

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
using modint = ModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

template <typename T>
vector<modint> convolve(vector<T> a, vector<T> b) {
    int n = 1;
    while (n < a.size() + b.size() - 1) n <<= 1;
    a.resize(n);
    b.resize(n);

    modint n_mod = n;
    auto w = pow(3ULL, (MODULO - 1) / n, MODULO), w_inv = pow(w, MODULO - 2, MODULO), n_inv = pow(n_mod.value, MODULO - 2, MODULO);
    vector<int> reverse(n, 0);
    for (int i = 1, j = __lg(n) - 1, k = -1; i < n; i++) {
        if (!(i & (i - 1))) k++;
        reverse[i] = reverse[i ^ (1 << k)] | (1 << (j - k));
    }

    auto ntt = [&](vector<modint> &v, modint omega) {
        for (int i = 0; i < n; i++)
            if (i < reverse[i]) swap(v[i], v[reverse[i]]);

        vector<modint> twiddles(n >> 1, 1);
        for (int i = 1; i < (n >> 1); i++) twiddles[i] = twiddles[i - 1] * omega;

        for (int i = 0; i < __lg(n); i++)
            for (int j = 0; j < n; j++)
                if (!(j & (1 << i))) {
                    auto t = v[j ^ (1 << i)] * twiddles[(j & ((1 << i) - 1)) * (n >> (i + 1))];
                    v[j ^ (1 << i)] = v[j] - t;
                    v[j] += t;
                }
    };
    ntt(a, w);
    ntt(b, w);

    vector<modint> c(n);
    for (int i = 0; i < n; i++) c[i] = a[i] * b[i];
    ntt(c, w_inv);
    for (auto &ci : c) ci *= n_inv;

    c.resize(a.size() + b.size() - 1);
    return c;
}

constexpr int OFFSET = 5e4;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int n;
    cin >> n;

    vector<int> a(n);
    vector<modint> count(4 * OFFSET);
    auto extra = 0;
    for (int &ai : a) {
        cin >> ai;

        if (!ai) extra++;
        count[ai += OFFSET]++;
    }

    auto c = convolve(count, count);
    for (int ai : a) c[2 * ai].value--;

    auto ways = 0ULL;
    for (int ai : a) ways += c[ai + OFFSET].value;
    ways -= 2LL * extra * (n - 1);

    cout << ways;
}
