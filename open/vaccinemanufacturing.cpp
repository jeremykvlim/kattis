#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

istream & operator>>(istream &stream, __int128 &x) {
    string s;
    stream >> s;

    x = 0;
    for (int sgn = s[0] == '-' ? -1 : 1, i = sgn < 0; i < s.size(); i++) x = x * 10 + sgn * (s[i] - '0');
    return stream;
}

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

    static mt19937_64 rng(random_device{}());
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

template <auto M>
struct MontgomeryModInt {
    using T = conditional_t<__lg(M) < 30, unsigned int, unsigned long long>;
    using U = conditional_t<is_same_v<T, unsigned int>, unsigned long long, unsigned __int128>;
    using I = conditional_t<is_same_v<T, unsigned int>, int, long long>;
    using J = conditional_t<is_same_v<T, unsigned int>, long long, __int128>;

    T value;

    constexpr static T mod() {
        return M;
    }

    static constexpr int p2 = countr_zero((T) (mod() - 1));
    static inline pair<T, U> r = [] {
        pair<T, U> r{(T) M, -(U) M % (T) M};
        while ((T) M * r.first != 1) r.first *= (T) 2 - (T) M * r.first;
        return r;
    }();
    static constexpr int bit_length = sizeof(T) * 8;
    static inline bool prime_mod = mod() == 998244353 || mod() == 1000000007 || mod() == 1000000009 || mod() == 1000069 || mod() == 2524775926340780033 || mod() == 39582418599937 || mod() == 79164837199873 || isprime(mod());

    constexpr MontgomeryModInt() : value() {}

    MontgomeryModInt(const J &x) {
        J v = x % mod();
        if (v < 0) v += mod();
        value = reduce((U) v * r.second);
    }

    template <auto N>
    MontgomeryModInt(const MontgomeryModInt<N> &x) {
        value = reduce((U) (x() % mod()) * r.second);
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
        return (V) (*this)();
    }

    I recover() const {
        T v = reduce((U) value);
        return v > mod() / 2 ? v - mod() : v;
    }

    static T primitive_root() {
        static const T g = primitive_root_mod_m(mod());
        return g;
    }

    static bool ntt_viable(int n) {
        return prime_mod && !(n & (n - 1)) && __lg(n) <= p2;
    }

    inline auto & operator+=(const MontgomeryModInt &v) {
        if ((I) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const MontgomeryModInt &v) {
        if ((I) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator+=(const V &v) {
        return *this += (MontgomeryModInt) v;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator-=(const V &v) {
        return *this -= (MontgomeryModInt) v;
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
        return (MontgomeryModInt) 0 - *this;
    }

    auto & operator*=(const MontgomeryModInt &v) {
        if constexpr (is_same_v<T, unsigned int>) value = reduce((unsigned long long) value * v.value);
        else value = reduce((unsigned __int128) value * v.value);
        return *this;
    }

    auto & operator/=(const MontgomeryModInt &v) {
        return *this *= inv(v);
    }

    friend bool operator==(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs.value == rhs.value;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(const MontgomeryModInt &lhs, V rhs) {
        return lhs == MontgomeryModInt(rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) == rhs;
    }

    friend bool operator!=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(const MontgomeryModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(V lhs, const MontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator>(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs() > rhs();
    }

    friend bool operator<(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs() < rhs();
    }

    friend bool operator>=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs > rhs || lhs == rhs;
    }

    friend bool operator<=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs < rhs || lhs == rhs;
    }

    friend MontgomeryModInt operator+(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator+(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator+(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    friend MontgomeryModInt operator-(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator-(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator-(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    friend MontgomeryModInt operator*(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator*(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator*(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    friend MontgomeryModInt operator/(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator/(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator/(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename S>
    friend S & operator<<(S &stream, const MontgomeryModInt &v) {
        return stream << v();
    }

    template <typename S>
    friend S & operator>>(S &stream, MontgomeryModInt &v) {
        J x;
        stream >> x;
        v = MontgomeryModInt(x);
        return stream;
    }

    template <typename V>
    static MontgomeryModInt pow(MontgomeryModInt base, V exponent) {
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

        J x = 0, y = 1;
        T a = v(), m = mod();
        while (a) {
            T t = m / a;
            m -= t * a;
            swap(a, m);
            x -= (J) t * y;
            swap(x, y);
        }

        return (MontgomeryModInt) x;
    }
};

constexpr int MOD = 1e9 + 7;
using modint = MontgomeryModInt<MOD>;

struct HashedString {
    static inline unsigned long long B1 = 0, B2 = 0;
    static const unsigned long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;

    int n;
    vector<unsigned long long> pref1, pref2;
    static inline vector<unsigned long long> p1{1}, p2{1};

    HashedString() : n(0), pref1(1, 0), pref2(1, 0) {}
    HashedString(const string &s) : n(s.size()), pref1(n + 1, 0), pref2(n + 1, 0) {
        if (!B1 && !B2) {
            mt19937_64 rng{random_device{}()};
            B1 = uniform_int_distribution(911382323ULL, MOD1 - 1)(rng);
            B2 = uniform_int_distribution(972663749ULL, MOD2 - 1)(rng);
        }
        while (p1.size() <= n || p2.size() <= n) {
            p1.emplace_back((p1.back() * B1) % MOD1);
            p2.emplace_back((p2.back() * B2) % MOD2);
        }
        for (int i = 0; i < n; i++) {
            auto v = (unsigned char) s[i] + 1;
            pref1[i + 1] = (pref1[i] * B1 + v) % MOD1;
            pref2[i + 1] = (pref2[i] * B2 + v) % MOD2;
        }
    }

    pair<unsigned long long, unsigned long long> pref_hash(int l, int r) const {
        auto h1 = (pref1[r] + MOD1 - (pref1[l] * p1[r - l]) % MOD1) % MOD1, h2 = (pref2[r] + MOD2 - (pref2[l] * p2[r - l]) % MOD2) % MOD2;
        return {h1, h2};
    }
};

pair<__int128, __int128> manacher(const string &s) {
    int n = s.size();
    if (!n) return {1, 1};

    __int128 pref_mask = 1, suff_mask = (__int128) 1 << n;
    vector<int> dp(2 * n - 1, 0);
    for (int k = 0, l = -1, r = -1; k < 2 * n - 1; k++) {
        int i = (k + 1) >> 1, j = k >> 1, p = (i >= r ? 0 : min(r - i, dp[2 * (l + r) - k]));
        while (j + p + 1 < n && i - p - 1 >= 0 && s[j + p + 1] == s[i - p - 1]) p++;

        if (i == p) pref_mask |= (__int128) 1 << (j + p + 1);
        if (j + p == n - 1) suff_mask |= (__int128) 1 << (i - p);

        if (r < j + p) {
            r = j + p;
            l = i - p;
        }

        dp[k] = p;
    }
    return {pref_mask, suff_mask};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<string, int> freq;
    while (n--) {
        string m;
        cin >> m;

        freq[m]++;
    }

    vector<string> m, rev;
    vector<HashedString> hs_m, hs_rev;
    vector<int> count;
    for (auto [mi, f] : freq) {
        m.emplace_back(mi);
        rev.emplace_back(mi);
        reverse(rev.back().begin(), rev.back().end());
        hs_m.emplace_back(mi);
        hs_rev.emplace_back(rev.back());
        count.emplace_back(f);
    }
    n = m.size();

    int sum = 0;
    vector<__int128> pref_mask(n, 0), suff_mask(n, 0);
    vector<vector<int>> pref_pos(n), suff_pos(n);
    for (int i = 0; i < n; i++) {
        auto [pref, suff] = manacher(m[i]);
        int len = m[i].size();
        for (int j = 0; j <= len; j++)
            if ((pref >> j) & 1) pref_mask[i] |= (__int128) 1 << (len - j);
        if (pref_mask[i] & 1) sum += count[i];
        suff_mask[i] = suff;
        for (int j = 0; j < len; j++) {
            if ((pref_mask[i] >> j) & 1) pref_pos[i].emplace_back(j);
            if ((suff_mask[i] >> j) & 1) suff_pos[i].emplace_back(j);
        }
    }

    unordered_map<pair<unsigned long long, unsigned long long>, int, Hash> exact_m, exact_rev, pal_m, pal_rev;
    for (int i = 0; i < n; i++) {
        int len = m[i].size();
        exact_m[hs_m[i].pref_hash(0, len)] += count[i];
        exact_rev[hs_rev[i].pref_hash(0, len)] += count[i];
        for (int j = 0; j <= len; j++) {
            if ((suff_mask[i] >> j) & 1) pal_m[hs_m[i].pref_hash(0, j)] += count[i];
            if ((pref_mask[i] >> j) & 1) pal_rev[hs_rev[i].pref_hash(0, j)] += count[i];
        }
    }

    modint ways = 0;
    for (int i = 0; i < n; i++) {
        int len = m[i].size();

        auto get = [&](const auto &c, auto p) {
            auto it = c.find(p);
            return it == c.end() ? 0 : it->second;
        };
        ways += (modint) sum * count[i] * get(exact_m, hs_rev[i].pref_hash(0, len));
        for (int l = 0; l < len; l++) {
            int end_m = get(exact_m, hs_rev[i].pref_hash(0, l));
            if (end_m) {
                int s = get(pal_m, hs_rev[i].pref_hash(l, len));
                for (auto it = lower_bound(pref_pos[i].begin(), pref_pos[i].end(), l); it != pref_pos[i].end(); it++) s += get(exact_m, hs_rev[i].pref_hash(l, *it));
                ways += (modint) s * count[i] * end_m;
            }

            int end_rev = get(exact_rev, hs_m[i].pref_hash(0, l));
            if (end_rev) {
                int s = get(pal_rev, hs_m[i].pref_hash(l, len));
                for (auto it = lower_bound(suff_pos[i].begin(), suff_pos[i].end(), l); it != suff_pos[i].end(); it++) s += get(exact_rev, hs_m[i].pref_hash(l, *it));
                ways += (modint) s * count[i] * end_rev;
            }
        }
    }

    cout << ways;
}