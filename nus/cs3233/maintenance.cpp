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

constexpr int MODULO = 1e9 + 7;
const bool PRIME_MOD = MODULO == 998244353 || MODULO == 1e9 + 7 || MODULO == 1e9 + 9 || MODULO == 1e6 + 69 || isprime(MODULO);

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

using modint = ModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

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

void divisors(vector<pair<int, int>> &pfs, vector<pair<double, long long>> &divs, pair<double, modint> d = {0, 1}, int i = 0) {
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

    vector<pair<double, long long>> divs1{{0, 1}}, divs2{{0, 1}};
    divisors(pfs1, divs1);
    divisors(pfs2, divs2);
    sort(divs1.begin(), divs1.end());
    sort(divs2.rbegin(), divs2.rend());

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
