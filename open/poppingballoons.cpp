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

template <typename M>
struct MontgomeryModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, unsigned __int128, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;
    using J = typename conditional<is_same<T, unsigned int>::value, long long, typename conditional<is_same<T, unsigned long long>::value, __int128, void>::type>::type;

    T value;
    static inline pair<T, U> r;
    static inline bool prime_mod;
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
        value = reduce((unsigned __int128) value * v.value);
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

    static MontgomeryModInt pow(MontgomeryModInt base, T exponent) {
        MontgomeryModInt v = 1;
        while (exponent) {
            if (exponent & 1) v *= base;
            base *= base;
            exponent >>= 1;
        }
        return v;
    }

    static MontgomeryModInt inv(const MontgomeryModInt &v) {
        if (prime_mod) return pow(v, mod() - 2);

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

template <typename T, typename U>
U & operator<<(U &stream, const MontgomeryModInt<T> &v) {
    return stream << v();
}

template <typename T, typename U>
U & operator>>(U &stream, MontgomeryModInt<T> &v) {
    typename common_type<typename MontgomeryModInt<T>::T, long long>::type x;
    stream >> x;
    v = MontgomeryModInt<T>(x);
    return stream;
}

constexpr unsigned long long MOD = 998244353;
using modint = MontgomeryModInt<integral_constant<decay<decltype(MOD)>::type, MOD>>;

static unsigned long long primitive_root() {
    if (MOD == 9223372036737335297 || MOD == 2524775926340780033 || MOD == 998244353 || MOD == 167772161) return 3ULL;
    if (MOD == 754974721) return 11ULL;
}

template <typename T, typename R>
void cooley_tukey(int n, vector<T> &v, R root) {
    static vector<int> rev;
    static vector<T> twiddles;

    if (rev.size() != n) {
        rev.resize(n);
        for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] | (i & 1) << __lg(n)) >> 1;
    }

    if (twiddles.size() < n) {
        int m = max(2, (int) twiddles.size());
        twiddles.resize(n, 1);

        for (int k = m; k < n; k <<= 1) {
            auto w = root(k);
            for (int i = k; i < k << 1; i++) twiddles[i] = i & 1 ? twiddles[i >> 1] * w : twiddles[i >> 1];
        }
    }

    for (int i = 0; i < n; i++)
        if (i < rev[i]) swap(v[i], v[rev[i]]);

    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) {
                auto t = v[i + j + k] * twiddles[j + k];
                v[i + j + k] = v[i + j] - t;
                v[i + j] += t;
            }
}

vector<modint> ntt(int n, const vector<modint> &f) {
    auto F = f;
    cooley_tukey(n, F, [](int k) { return modint::pow(primitive_root(), (MOD - 1) / (k << 1)); });
    return F;
}

vector<modint> intt(int n, const vector<modint> &F) {
    auto f = F;
    cooley_tukey(n, f, [](int k) { return modint::pow(primitive_root(), (MOD - 1) / (k << 1)); });
    auto n_inv = modint::inv(n);
    for (auto &v : f) v *= n_inv;
    reverse(f.begin() + 1, f.end());
    return f;
}

template <typename T>
vector<T> convolve(const vector<T> &a, const vector<T> &b) {
    int da = a.size(), db = b.size(), m = da + db - 1, n = bit_ceil((unsigned) m);
    if (n <= 64 || min(da, db) <= __lg(n)) {
        vector<modint> p(da), q(db);
        for (int i = 0; i < da; i++) p[i] = a[i];
        for (int i = 0; i < db; i++) q[i] = b[i];
        if (da > db) {
            swap(p, q);
            swap(da, db);
        }

        vector<T> r(m, 0);
        for (int i = 0; i < db; i++) r[i] = q[i]();
        for (int i = m - 1; ~i; i--) {
            modint v = 0;
            for (int j = max(0, i - (db - 1)); j <= min(i, da - 1); j++) v += p[j] * r[i - j];
            r[i] = v();
        }
        return r;
    }

    vector<modint> ntt_a(n);
    for (int i = 0; i < da; i++) ntt_a[i] = a[i];

    vector<modint> F_a = ntt(n, ntt_a), F_b;
    if (a == b) F_b = F_a;
    else {
        vector<modint> ntt_b(n);
        for (int i = 0; i < db; i++) ntt_b[i] = b[i];
        F_b = ntt(n, ntt_b);
    }

    vector<modint> F_c(n);
    for (int i = 0; i < n; i++) F_c[i] = F_a[i] * F_b[i];
    auto f_c = intt(n, F_c);

    vector<T> c(m);
    for (int i = 0; i < m; i++) c[i] = f_c[i]();
    return c;
}

template <typename T>
vector<T> binomial_transform(const vector<T> &a, const vector<T> &fact, const vector<T> &fact_inv) {
    int n = a.size();
    vector<T> x(n), y(n);
    for (int i = 0; i < n; i++) {
        x[i] = a[i] * fact_inv[i];
        y[i] = fact_inv[i];
    }
    auto c = convolve(x, y);

    vector<T> b(n);
    for (int k = 0; k < n; k++) b[k] = fact[k] * c[k];
    return b;
}

template <typename T>
T binomial_coefficient_mod_p(long long n, long long k, int p, vector<T> &fact, vector<T> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return binomial_coefficient_mod_p(n / p, k / p, p, fact, fact_inv) *
                                 binomial_coefficient_mod_p(n % p, k % p, p, fact, fact_inv);
    return fact[n] * fact_inv[k] * fact_inv[n - k];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    string s;
    cin >> s;

    int m = s.size();
    vector<int> pref_b(m + 1, 0), pref_y(m + 1, 0), suff_r(m, 0);
    for (int i = 0; i < m; i++) {
        pref_b[i + 1] = pref_b[i] + (s[i] == 'B');
        pref_y[i + 1] = pref_y[i] + (s[i] == 'Y');
    }
    for (int i = m - 1; i; i--) suff_r[i - 1] = suff_r[i] + (s[i] == 'R');

    vector<modint> fact(m + 1, 1), fact_inv(m + 1, 1);
    auto prepare = [&]() {
        auto inv = fact;

        for (int i = 1; i <= m; i++) {
            if (i > 1) inv[i] = (MOD - MOD / i) * inv[MOD % i];
            fact[i] = i * fact[i - 1];
            fact_inv[i] = inv[i] * fact_inv[i - 1];
        }
    };
    prepare();

    vector<modint> count(m + 1), total(m + 1);
    auto dnc = [&](auto &&self, int l, int r) -> void {
        if (l + 1 == r) return;

        int mid = l + (r - l) / 2, ll = 1e9, lr = -1e9, rl = 1e9, rr = -1e9;
        for (int i = l; i < mid; i++)
            if (s[i] == 'B') {
                int v = pref_b[i] - pref_y[i + 1];
                ll = min(ll, v);
                lr = max(lr, v);
            }
        for (int i = mid; i < r; i++)
            if (s[i] == 'R') {
                int v = suff_r[i] + pref_y[i + 1];
                rl = min(rl, v);
                rr = max(rr, v);
            }

        if (ll <= lr && rl <= rr) {
            vector<modint> a(lr - ll + 1), b(rr - rl + 1);
            for (int i = l; i < mid; i++)
                if (s[i] == 'B') a[pref_b[i] - pref_y[i + 1] - ll]++;
            for (int i = mid; i < r; i++)
                if (s[i] == 'R') b[suff_r[i] + pref_y[i + 1] - rl]++;
            auto c = convolve(a, b);
            for (int i = 0; i < c.size(); i++) count[i + ll + rl] += c[i];
        }

        self(self, l, mid);
        self(self, mid, r);
    };
    dnc(dnc, 0, m);

    auto transform = [&](const vector<modint> &count) {
        vector<modint> a(m + 1);
        for (int i = 0; i <= m; i++) a[i] = count[m - i] * fact[m - i] * fact[i];
        auto b = binomial_transform(a, fact, fact_inv);

        vector<modint> c(m + 1);
        for (int i = 0; i <= m; i++) c[i] = b[m - i] * fact_inv[m - i] * fact_inv[i];
        return c;
    };

    auto c = transform(count);
    for (int i = 2; i <= m; i++) total[i] += c[i - 2];

    int n = pref_y[m];
    fill(count.begin(), count.end(), 0);
    for (int i = 0; i < m; i++) {
        if (s[i] == 'B') count[pref_b[i] - pref_y[i + 1] + n]++;
        if (s[i] == 'R') count[suff_r[i] + pref_y[i + 1]]++;
    }
    c = transform(count);
    for (int i = 1; i <= m; i++) total[i] += c[i - 1];

    for (int k = 1; k <= n; k++) total[k] += binomial_coefficient_mod_p(n, k, MOD, fact, fact_inv);

    modint t = m;
    for (int i = 1; i <= m; i++) t -= fact[i] * fact[m - i] * fact_inv[m] * total[i];
    cout << t;
}
