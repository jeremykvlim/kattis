#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

istream & operator>>(istream &stream, __int128 &x) {
    string s;
    stream >> s;

    x = 0;
    for (int sgn = s[0] == '-' ? -1 : 1, i = sgn < 0; i < s.size(); i++) x = x * 10 + sgn * (s[i] - '0');
    return stream;
}

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

template <typename T>
struct DynamicMontgomeryModInt {
    using U = conditional_t<is_same_v<T, unsigned int>, unsigned long long, unsigned __int128>;
    using I = conditional_t<is_same_v<T, unsigned int>, int, long long>;
    using J = conditional_t<is_same_v<T, unsigned int>, long long, __int128>;

    T value;
    static inline T modulus;

    static T mod() {
        return modulus;
    }

    static inline pair<T, U> r;
    static inline bool prime_mod;
    static constexpr int bit_length = sizeof(T) * 8;

    static void init(T m) {
        modulus = m;
        prime_mod = mod() == 998244353 || mod() == 1e9 + 7 || mod() == 1e9 + 9 || mod() == 1e6 + 69 || mod() == 2524775926340780033 || mod() == 39582418599937 || mod() == 79164837199873 || isprime(mod());
        r = {mod(), -(U) mod() % mod()};
        while (mod() * r.first != 1) r.first *= (T) 2 - mod() * r.first;
    }

    constexpr DynamicMontgomeryModInt() : value() {}

    DynamicMontgomeryModInt(const J &x) {
        J v = x % mod();
        if (v < 0) v += mod();
        value = reduce((U) v * r.second);
    }

    static T reduce(const U &x) {
        T q = x * r.first, v = (x >> bit_length) + mod() - (((U) q * mod()) >> bit_length);
        return v >= mod() ? v - mod() : v;
    }

    T operator()() const {
        return reduce((U) value);
    }

    template <typename V>
    explicit operator V() const {
        return (V) (*this)();
    }

    I recover() const {
        T v = reduce((U) value);
        return v > mod() / 2 ? v - mod() : v;
    }

    inline auto & operator+=(const DynamicMontgomeryModInt &v) {
        if ((I) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const DynamicMontgomeryModInt &v) {
        if ((I) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator+=(const V &v) {
        return *this += (DynamicMontgomeryModInt) v;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator-=(const V &v) {
        return *this -= (DynamicMontgomeryModInt) v;
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
        return (DynamicMontgomeryModInt) 0 - *this;
    }

    auto & operator*=(const DynamicMontgomeryModInt &v) {
        if constexpr (is_same_v<T, unsigned int>) value = reduce((unsigned long long) value * v.value);
        else value = reduce((unsigned __int128) value * v.value);
        return *this;
    }

    auto & operator/=(const DynamicMontgomeryModInt &v) {
        return *this *= inv(v);
    }

    friend bool operator==(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return lhs.value == rhs.value;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(const DynamicMontgomeryModInt &lhs, V rhs) {
        return lhs == DynamicMontgomeryModInt(rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(V lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) == rhs;
    }

    friend bool operator!=(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(const DynamicMontgomeryModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(V lhs, const DynamicMontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator>(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return lhs() > rhs();
    }

    friend bool operator<(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return lhs() < rhs();
    }

    friend bool operator>=(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return lhs > rhs || lhs == rhs;
    }

    friend bool operator<=(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return lhs < rhs || lhs == rhs;
    }

    friend DynamicMontgomeryModInt operator+(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator+(const DynamicMontgomeryModInt &lhs, V rhs) {
        return DynamicMontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator+(V lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) += rhs;
    }

    friend DynamicMontgomeryModInt operator-(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator-(const DynamicMontgomeryModInt &lhs, V rhs) {
        return DynamicMontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator-(V lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) -= rhs;
    }

    friend DynamicMontgomeryModInt operator*(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator*(const DynamicMontgomeryModInt &lhs, V rhs) {
        return DynamicMontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator*(V lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) *= rhs;
    }

    friend DynamicMontgomeryModInt operator/(const DynamicMontgomeryModInt &lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator/(const DynamicMontgomeryModInt &lhs, V rhs) {
        return DynamicMontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicMontgomeryModInt operator/(V lhs, const DynamicMontgomeryModInt &rhs) {
        return DynamicMontgomeryModInt(lhs) /= rhs;
    }

    template <typename S>
    friend S & operator<<(S &stream, const DynamicMontgomeryModInt &v) {
        return stream << v();
    }

    template <typename S>
    friend S & operator>>(S &stream, DynamicMontgomeryModInt &v) {
        J x;
        stream >> x;
        v = DynamicMontgomeryModInt(x);
        return stream;
    }

    template <typename V>
    static DynamicMontgomeryModInt pow(DynamicMontgomeryModInt base, V exponent) {
        DynamicMontgomeryModInt v = 1;
        while (exponent) {
            if (exponent & 1) v *= base;
            base *= base;
            exponent >>= 1;
        }
        return v;
    }

    static DynamicMontgomeryModInt inv(const DynamicMontgomeryModInt &v) {
        if (prime_mod) return pow(v, mod() - 2);

        J x = 0, y = 1;
        T a = v(), m = mod();
        while (a) {
            T t = m / a;
            m -= t * a;
            swap(a, m);
            x -= (J) t * y;
            swap(x, y);
        }

        return (DynamicMontgomeryModInt) x;
    }
};

using modint = DynamicMontgomeryModInt<unsigned long long>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long x, p;
    cin >> x >> p;

    if (p == 2) {
        cout << "0 0";
        exit(0);
    }

    modint::init(p);

    gp_hash_table<long long, int> pos;
    pos[1] = 0;
    modint fact = 1;
    for (int i = 1; i <= 1e6; i++) {
        fact *= i;
        if (pos.find(fact()) == pos.end()) pos[fact()] = i;
    }

    for (int r = sqrt(p - 1) + 1;; r = r << 1 | 1) {
        modint f = 1;
        for (int i = 1; i <= r; f *= i++) {
            fact = f * x;
            if (i & 1) fact = -fact;
            if (pos.find(fact()) != pos.end()) {
                cout << pos[fact()] << " " << p - i;
                exit(0);
            }
        }
    }
}
