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

template <typename T>
T brent(T n) {
    if (!(n & 1)) return 2;

    mt19937_64 rng(random_device{}());
    for (;;) {
        T x = 2, y = 2, g = 1, q = 1, xs = 1, c = rng() % (n - 1) + 1;
        for (int i = 1; g == 1; i <<= 1, y = x) {
            for (int j = 1; j < i; j++) x = mul(x, x, n) + c;
            for (int j = 0; j < i && g == 1; j += 128) {
                xs = x;
                for (int k = 0; k < min(128, i - j); k++) {
                    x = mul(x, x, n) + c;
                    q = mul(q, max(x, y) - min(x, y), n);
                }
                g = __gcd(q, n);
            }
        }

        if (g == n) g = 1;
        while (g == 1) {
            xs = mul(xs, xs, n) + c;
            g = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (g != n) return isprime(g) ? g : brent(g);
    }
}

template <typename T>
vector<T> factorize(T n) {
    vector<T> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs.emplace_back(m);
            return;
        }

        T pf = brent(m);
        pfs.emplace_back(pf);
        self(self, m / pf);
    };
    dfs(dfs, n);

    return pfs;
}

template <typename T>
T primitive_root_mod_m(T m) {
    if (m == 1 || m == 2 || m == 4) return m - 1;
    if (!(m & 3)) return m;

    auto pfs = factorize(m);
    sort(pfs.begin(), pfs.end());
    pfs.erase(unique(pfs.begin(), pfs.end()), pfs.end());
    if (pfs.size() > 2 || (pfs.size() == 2 && (m & 1))) return m;

    auto phi = !(m & 1) ? m / 2 / pfs[1] * (pfs[1] - 1) : m / pfs[0] * (pfs[0] - 1);
    pfs = factorize(phi);
    sort(pfs.begin(), pfs.end());
    pfs.erase(unique(pfs.begin(), pfs.end()), pfs.end());
    for (auto g = 2LL; g < m; g++)
        if (gcd(g, m) == 1 && all_of(pfs.begin(), pfs.end(), [&](auto pf) { return pow((T) g, phi / pf, m) != 1; })) return g;

    return m;
}

template <typename M>
struct MontgomeryModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, unsigned __int128, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;
    using J = typename conditional<is_same<T, unsigned int>::value, long long, typename conditional<is_same<T, unsigned long long>::value, __int128, void>::type>::type;

    T value;
    static inline int p2;
    static inline T g;
    static inline pair<T, U> r;
    static inline bool prime_mod;
    static constexpr int bit_length = sizeof(T) * 8;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == (unsigned long long) 1e9 + 7 || mod() == (unsigned long long) 1e9 + 9 || mod() == (unsigned long long) 1e6 + 69 || mod() == 2524775926340780033 || isprime(mod());
        r = {mod(), - (U) mod() % mod()};
        while (mod() * r.first != 1) r.first *= (T) 2 - mod() * r.first;
        g = primitive_root_mod_m(mod());
        p2 = 0;
        for (T t = mod() - 1; !(t & 1); t >>= 1) p2++;
    }

    constexpr MontgomeryModInt() : value() {}

    MontgomeryModInt(const J &x) {
        value = reduce((U) x * r.second);
    }

    template <typename N, typename = enable_if_t<!is_same<M, N>::value && is_same<typename MontgomeryModInt<N>::T, T>::value>>
    MontgomeryModInt(const MontgomeryModInt<N> &x) {
        value = reduce((U) x() * r.second);
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

    I recover() const {
        T v = reduce((U) value);
        return v > mod() / 2 ? v - mod() : v;
    }

    constexpr static T mod() {
        return M::value;
    }

    constexpr static T primitive_root() {
        return g;
    }

    constexpr static bool ntt_viable(int n) {
        if (!prime_mod || (n & (n - 1)) || g == mod()) return false;
        return __lg(n) <= p2;
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

    MontgomeryModInt & operator*=(const MontgomeryModInt &v) {
        if constexpr (is_same_v<T, unsigned int>) value = reduce((unsigned long long) value * v.value);
        else value = reduce((unsigned __int128) value * v.value);
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
    typename make_signed<typename MontgomeryModInt<T>::T>::type x;
    stream >> x;
    v = MontgomeryModInt<T>(x);
    return stream;
}

constexpr unsigned long long MOD = 1e9 + 7;
using modint = MontgomeryModInt<integral_constant<decay<decltype(MOD)>::type, MOD>>;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(m, n);
    if ((b - a) % g) return {0, -1};

    T n0 = n / g, m0 = m / g, lcm = n0 * m;
    auto [x, y] = bezout(n0, m0);
    T r = ((__int128) n * (((__int128) ((b - a) / g) * x % m0 + m0) % m0) + a) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
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

template <typename M>
vector<M> ntt(int n, const vector<M> &f) {
    auto F = f;
    cooley_tukey(n, F, [](int k) { return M::pow(M::primitive_root(), (M::mod() - 1) / (k << 1)); });
    return F;
}

template <typename M>
vector<M> intt(int n, const vector<M> &F) {
    auto f = F;
    cooley_tukey(n, f, [](int k) { return M::pow(M::primitive_root(), (M::mod() - 1) / (k << 1)); });
    auto n_inv = M::inv(n);
    for (auto &v : f) v *= n_inv;
    reverse(f.begin() + 1, f.end());
    return f;
}

template <typename T>
vector<T> convolve(const vector<T> &a, const vector<T> &b) {
    int da = a.size(), db = b.size(), m = da + db - 1, n = bit_ceil((unsigned) m);
    if (n <= 256 || min(da, db) <= __lg(n)) {
        vector<modint> x(da), y(db);
        for (int i = 0; i < da; i++) x[i] = a[i];
        for (int i = 0; i < db; i++) y[i] = b[i];
        if (da > db) {
            swap(x, y);
            swap(da, db);
        }

        vector<T> z(m);
        for (int i = 0; i < m; i++) {
            int l = max(0, i - (db - 1)), r = min(i, da - 1) + 1;
            z[i] = inner_product(x.begin() + l, x.begin() + r, make_reverse_iterator(y.begin() + (i - l + 1)), (modint) 0).recover();
        }
        return z;
    }

    if (!modint::ntt_viable(n)) {
        constexpr unsigned long long ntt_mod1 = 39582418599937, ntt_mod2 = 79164837199873;
        using ntt_modint1 = MontgomeryModInt<integral_constant<decay<decltype(ntt_mod1)>::type, ntt_mod1>>;
        using ntt_modint2 = MontgomeryModInt<integral_constant<decay<decltype(ntt_mod2)>::type, ntt_mod2>>;

        ntt_modint1::init();
        ntt_modint2::init();

        vector<ntt_modint1> f_a1(n), f_b1(n);
        vector<ntt_modint2> f_a2(n), f_b2(n);
        for (int i = 0; i < da; i++) {
            f_a1[i] = a[i];
            f_a2[i] = a[i];
        }
        for (int i = 0; i < db; i++) {
            f_b1[i] = b[i];
            f_b2[i] = b[i];
        }

        auto F_a1 = ntt(n, f_a1), F_b1 = ntt(n, f_b1);
        auto F_a2 = ntt(n, f_a2), F_b2 = ntt(n, f_b2);

        vector<ntt_modint1> F_c1(n);
        vector<ntt_modint2> F_c2(n);
        for (int i = 0; i < n; i++) {
            F_c1[i] = F_a1[i] * F_b1[i];
            F_c2[i] = F_a2[i] * F_b2[i];
        }
        auto f_c1 = intt(n, F_c1);
        auto f_c2 = intt(n, F_c2);

        vector<T> c(m);
        for (int i = 0; i < m; i++) c[i] = modint{chinese_remainder_theorem<__int128>(f_c1[i](), ntt_mod1, f_c2[i](), ntt_mod2).first}.recover();
        return c;
    }

    vector<modint> f_a(n);
    for (int i = 0; i < da; i++) f_a[i] = a[i];

    vector<modint> F_a = ntt(n, f_a), F_b;
    if (a == b) F_b = F_a;
    else {
        vector<modint> f_b(n);
        for (int i = 0; i < db; i++) f_b[i] = b[i];
        F_b = ntt(n, f_b);
    }

    vector<modint> F_c(n);
    for (int i = 0; i < n; i++) F_c[i] = F_a[i] * F_b[i];
    auto f_c = intt(n, F_c);

    vector<T> c(m);
    for (int i = 0; i < m; i++) c[i] = f_c[i].recover();
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    long long n;
    int p, k;
    cin >> n >> p >> k;

    if (n == 1) {
        int count = 0;
        for (int d = 0; d <= 9; d++) count += (d % p == k);
        cout << count;
        exit(0);
    }

    auto half = n / 2;
    if (p == 2 || p == 5) {
        int count = 0;
        for (int d = 1; d <= 9; d++) count += (d % p == k);
        cout << modint::pow(10, half - !(n & 1)) * count;
        exit(0);
    }

    int m = 1;
    vector<int> p10{1 % p};
    for (int power = 10 % p; power != 1; power = mul(power, 10, p), m++) p10.emplace_back(power);

    vector<int> weights((min(half, (long long) m + 1)) + 1, 0);
    for (int i = 1, l = 0, r = (n - 1) % m; i < weights.size(); i++, ++l %= m, (--r += m) %= m) weights[i] = (p10[l] + p10[r]) % p;

    vector<modint> dp(p, 0);
    for (int d = 1; d <= 9; d++) dp[mul(d, weights[1], p)]++;

    auto cyclic_convolve = [&](const vector<modint> &a, const vector<modint> &b) {
        auto c = convolve(a, b);

        vector<modint> cyclic(p);
        for (int k = 0; k < p; k++) {
            cyclic[k] = c[k];
            if (k + p < c.size()) cyclic[k] += c[k + p];
        }
        return cyclic;
    };

    vector<int> shift(10);
    auto update = [&](const vector<modint> &v, int w) {
        for (int d = 0; d <= 9; d++) shift[d] = mul(d, w, p);

        vector<modint> u(p, 0);
        for (int i = 0; i < p; i++)
            if (v[i])
                for (int d = 0; d <= 9; d++) u[(i + shift[d]) % p] += v[i];
        return u;
    };

    auto remaining = half - 1;
    if (remaining) {
        if (remaining < m) {
            vector<modint> temp(p, 0);
            temp[0] = 1;
            for (int i = 2; i <= half; i++) temp = update(temp, weights[i]);
            dp = cyclic_convolve(dp, temp);
        } else {
            int r = remaining % m;
            vector<modint> cycle(p, 0);
            cycle[0] = 1;
            for (int i = 0; i < r; i++) cycle = update(cycle, weights[i + 2]);
            auto temp = cycle;
            for (int i = r; i < m; i++) cycle = update(cycle, weights[i + 2]);

            auto pow = [&](vector<modint> base, auto exponent) {
                vector<modint> value(p, 0);
                value[0] = 1;
                while (exponent) {
                    if (exponent & 1) value = cyclic_convolve(base, value);
                    base = cyclic_convolve(base, base);
                    exponent >>= 1;
                }
                return value;
            };
            dp = cyclic_convolve(dp, pow(cycle, remaining / m));
            if (r) dp = cyclic_convolve(dp, temp);
        }
    }

    if (n & 1) {
        vector<modint> temp(p, 0);
        for (int d = 0; d <= 9; d++) temp[mul(d, p10[half % m], p)]++;
        dp = cyclic_convolve(dp, temp);
    }
    cout << dp[k];
}
