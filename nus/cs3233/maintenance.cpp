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

constexpr unsigned long long MODULO = 1e9 + 7;
const bool PRIME_MOD = MODULO == 998244353 || MODULO == 1e9 + 7 || MODULO == 1e9 + 9 || MODULO == 1e6 + 69 || MODULO == 2524775926340780033 || isprime(MODULO);

template <typename M>
struct MontgomeryModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, __uint128_t, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;
    using J = typename conditional<is_same<T, unsigned int>::value, long long, typename conditional<is_same<T, unsigned long long>::value, __int128, void>::type>::type;

    T value;
    static pair<T, U> r;
    static constexpr int bit_length = sizeof(T) * 8;

    static void init() {
        r = {mod(), - (U) mod() % mod()};
        while (mod() * r.first != 1) r.first *= (T) 2 - mod() * r.first;
    }

    constexpr MontgomeryModInt() : value() {}

    MontgomeryModInt(const J &x) {
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
        return (MontgomeryModInt) -value;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, int>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned int>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((unsigned long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, long long>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((__int128) value * v.value);
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
        if (PRIME_MOD) {
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
    return lhs.value > rhs.value;
}

template <typename T>
bool operator<(const MontgomeryModInt<T> &lhs, const MontgomeryModInt<T> &rhs) {
    return lhs.value < rhs.value;
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
using modint = MontgomeryModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

vector<int> sieve(int n) {
    vector<int> factors(n + 1), primes{2};
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; p <= n; p += 2)
        if (factors[p] == p) {
            primes.emplace_back(p);
            for (auto i = (long long) p * p; i <= n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

void divisors(vector<pair<int, int>> &pfs, vector<pair<double, modint>> &divs, pair<double, modint> d = {0, 1}, int i = 0) {
    if (i == pfs.size()) return;

    divisors(pfs, divs, d, i + 1);

    auto [pf, pow] = pfs[i];
    pair<double, modint> p{log(pf), pf};
    while (pow--) {
        d = {d.first + p.first, d.second * p.second};
        divs.emplace_back(d);
        divisors(pfs, divs, d, i + 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();
    auto primes = sieve(99);
    vector<int> indices(primes.back() + 1), factors(primes.back() + 1, 0);
    for (int i = 0; i < primes.size(); i++) indices[primes[i]] = i;

    string s;
    cin >> s;

    pair<double, modint> K{0, 1};
    auto &[logk, kmod] = K;
    for (int i = 0; i < s.size() / 2; i++) {
        int pf = stoi(s.substr(2 * i, 2));
        logk += log(pf);
        kmod *= pf;
        factors[indices[pf]]++;
    }

    vector<pair<int, int>> pfs1, pfs2;
    auto s1 = 1LL, s2 = 1LL;
    for (int i = 0; i < factors.size(); i++) {
        if (!factors[i]) continue;

        (s1 <= s2 ? pfs1 : pfs2).emplace_back(primes[i], factors[i]);
        (s1 <= s2 ? s1 : s2) *= factors[i] + 1;
    }

    vector<pair<double, modint>> divs1{{0, 1}}, divs2{{0, 1}};
    divisors(pfs1, divs1);
    divisors(pfs2, divs2);
    sort(divs1.begin(), divs1.end(), [](auto p1, auto p2) {return p1.first < p2.first;});
    sort(divs2.rbegin(), divs2.rend(), [](auto p1, auto p2) {return p1.first < p2.first;});

    auto diff = DBL_MAX;
    modint d;
    for (int i = 0, j = 0; i < divs1.size(); i++) {
        while (divs1[i].first + divs2[j].first > 0.5 * logk && j < divs2.size() - 1) j++;

        for (int k = j - (j > 0); k <= j; k++)
            if (diff > abs(divs1[i].first + divs2[k].first - 0.5 * logk)) {
                diff = abs(divs1[i].first + divs2[k].first - 0.5 * logk);
                d = divs1[i].second * divs2[k].second;
            }
    }

    cout << d + kmod / d;
}
