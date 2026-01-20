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

constexpr unsigned long long MOD = 998244353;
using modint = MontgomeryModInt<integral_constant<decay<decltype(MOD)>::type, MOD>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int x, n, m;
    cin >> x >> n >> m;
    n += m;

    vector<int> p(n + 1), type(n + 1, 1), pref_t(n + 1, 0);
    vector<long long> pref_tp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        char t;
        cin >> p[i] >> t;

        if (t == 'P') type[i] = -1;
        pref_t[i] = pref_t[i - 1] + type[i];
        pref_tp[i] = pref_tp[i - 1] + type[i] * p[i];
    }

    vector<int> prev(n + 1), indices(n + 1, -1);
    for (int i = 0; i <= n; i++) {
        prev[i] = indices[pref_t[i] + m];
        indices[pref_t[i] + m] = i;
    }

    vector<pair<long long, modint>> dp(n + 1);
    auto add = [&](auto &p1, const pair<long long, modint> &p2) {
        if (p1.first == p2.first) p1.second += p2.second;
        else p1 = min(p1, p2);
    };

    auto compute = [&](int mid) {
        fill(dp.begin(), dp.end(), make_pair(1e18, 0));

        int l = 0, r = n;
        for (; l <= n && pref_t[l] != mid; l++);
        for (; ~r && pref_t[r] != mid + pref_t[n]; r--);
        if (l > r) return dp[n];

        dp[l] = {0, 1};
        for (int i = l + 1; i <= r; i++) {
            if (type[i] == 1) add(dp[i], dp[i - 1]);
            if (prev[i] >= l) {
                auto [val, count] = dp[prev[i]];
                val += abs(pref_tp[i] - pref_tp[prev[i]]);
                add(dp[i], {val, count});
            }
        }
        dp[r].first += abs(pref_tp[l] + pref_tp[n] - pref_tp[r] + (long long) x * mid);
        return dp[r];
    };

    int l = 0, r = 0, mid;
    for (int i = 1; i <= n; i++) {
        l = min(l, pref_t[i]);
        r = max(r, pref_t[i]);
    }
    r -= pref_t[n];
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        if (compute(mid) < compute(mid + 1)) r = mid;
        else l = mid;
    }

    pair<long long, modint> minimum{1e18, 0};
    for (mid = l - 2; mid <= r + 2; mid++) add(minimum, compute(mid));
    auto [val, count] = minimum;
    cout << val << "\n" << count;
}
