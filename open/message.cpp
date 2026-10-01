#include <bits/stdc++.h>
using namespace std;

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
struct DynamicModInt {
    using U = conditional_t<is_same_v<T, unsigned int>, unsigned long long, unsigned __int128>;
    using I = conditional_t<is_same_v<T, unsigned int>, int, long long>;
    using J = conditional_t<is_same_v<T, unsigned int>, long long, __int128>;

    T value;
    static inline T modulus;
    static inline bool prime_mod;

    static T mod() {
        return modulus;
    }

    static void init(T m) {
        modulus = m;
        prime_mod = mod() == 998244353 || mod() == 1000000007 || mod() == 1000000009 || mod() == 1000069 || mod() == 2524775926340780033 || mod() == 39582418599937 || mod() == 79164837199873 || isprime(mod());
    }

    constexpr DynamicModInt() : value() {}

    template <typename V>
    requires numeric_limits<V>::is_integer
    DynamicModInt(const V &x) {
        value = normalize((J) x);
    }

    static T normalize(J x) {
        x %= (J) mod();
        return x < 0 ? x + mod() : x;
    }

    const T & operator()() const {
        return value;
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    inline auto & operator+=(const DynamicModInt &v) {
        if ((I) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const DynamicModInt &v) {
        if ((I) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator+=(const V &v) {
        return *this += DynamicModInt(v);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator-=(const V &v) {
        return *this -= DynamicModInt(v);
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
        return (DynamicModInt) 0 - *this;
    }

    auto & operator*=(const DynamicModInt &v) {
        value = (U) value * v.value % mod();
        return *this;
    }

    friend bool operator==(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return lhs.value == rhs.value;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(const DynamicModInt &lhs, V rhs) {
        return lhs == DynamicModInt(rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(V lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) == rhs;
    }

    friend bool operator!=(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(const DynamicModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(V lhs, const DynamicModInt &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator>(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return lhs.value > rhs.value;
    }

    friend bool operator<(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return lhs.value < rhs.value;
    }

    friend bool operator>=(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return lhs > rhs || lhs == rhs;
    }

    friend bool operator<=(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return lhs < rhs || lhs == rhs;
    }

    friend DynamicModInt operator+(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator+(const DynamicModInt &lhs, V rhs) {
        return DynamicModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator+(V lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) += rhs;
    }

    friend DynamicModInt operator-(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator-(const DynamicModInt &lhs, V rhs) {
        return DynamicModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator-(V lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) -= rhs;
    }

    friend DynamicModInt operator*(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator*(const DynamicModInt &lhs, V rhs) {
        return DynamicModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator*(V lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) *= rhs;
    }

    friend DynamicModInt operator/(const DynamicModInt &lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator/(const DynamicModInt &lhs, V rhs) {
        return DynamicModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicModInt operator/(V lhs, const DynamicModInt &rhs) {
        return DynamicModInt(lhs) /= rhs;
    }

    template <typename S>
    friend S & operator<<(S &stream, const DynamicModInt &v) {
        return stream << v();
    }

    template <typename S>
    friend S & operator>>(S &stream, DynamicModInt &v) {
        J x;
        stream >> x;
        v = DynamicModInt(x);
        return stream;
    }

    auto & operator/=(const DynamicModInt &v) {
        return *this *= inv(v);
    }

    template <typename V>
    static DynamicModInt pow(DynamicModInt base, V exponent) {
        DynamicModInt v = 1;
        while (exponent) {
            if (exponent & 1) v *= base;
            base *= base;
            exponent >>= 1;
        }
        return v;
    }

    static DynamicModInt inv(const DynamicModInt &v) {
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

        return (DynamicModInt) x;
    }
};

using modint = DynamicModInt<unsigned long long>;

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
vector<pair<T, pair<T, int>>> factorize(T n) {
    unordered_map<T, pair<T, int>> pfs;

    auto add = [&](T pf) {
        auto it = pfs.find(pf);
        if (it == pfs.end()) pfs.emplace(pf, make_pair(pf, 1));
        else {
            it->second.first *= pf;
            it->second.second++;
        }
    };

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            add(m);
            return;
        }

        T pf = brent(m);
        add(pf);
        self(self, m / pf);
    };
    dfs(dfs, n);

    return {pfs.begin(), pfs.end()};
}

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    T q = a / b, r = a - q * b;
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(m, n);
    if ((b - a) % g) return {0, -1};

    T n0 = n / g, m0 = m / g, lcm = n0 * m;
    auto [_, x, y] = extended_gcd(n0, m0);
    T r = ((__int128) n * (((__int128) ((b - a) / g) * x % m0 + m0) % m0) + a) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}

vector<modint> reeds_sloane(const vector<modint> &S) {
    using U = decltype(modint::modulus);
    using T = make_signed_t<U>;

    int n = 0;
    U temp = modint::mod();
    auto pfs = factorize(temp);
    vector<vector<U>> coeffs;
    for (auto &[pf, pows] : pfs) {
        auto [pp, e] = pows;
        modint::init(pp);

        vector<U> pw(e, 1);
        for (int i = 1; i < e; i++) pw[i] = pw[i - 1] * pf;

        vector<vector<modint>> a(e), b(e), a_new(e), b_new(e), a_old(e), b_old(e);
        vector<modint> theta(e), theta_old(e);
        vector<int> u(e), u_old(e), r(e);
        auto normalize = [&](int i, modint d) {
            if (!d) {
                theta[i] = 1;
                u[i] = e;
                return false;
            }

            U discrepancy = d();
            u[i] = 0;
            while (!(discrepancy % pf)) {
                discrepancy /= pf;
                u[i]++;
            }
            theta[i] = discrepancy;
            return true;
        };

        for (int i = 0; i < e; i++) {
            a[i] = a_new[i] = {pw[i]};
            b[i] = {0};
            b_new[i] = {theta[i] = S[0] * pw[i]};
            normalize(i, theta[i]);
        }

        auto L = [&](const auto &a, const auto &b) {
            auto degree = [&](const auto &poly) {
                return poly.size() > 1 || (poly.size() == 1 && poly[0]) ? poly.size() - 1 : -1;
            };
            return max(degree(a), degree(b) + 1);
        };

        for (int k = 1; k < S.size(); k++) {
            for (int g = 0; g < e; g++)
                if (L(a_new[g], b_new[g]) > L(a[g], b[g])) {
                    int h = e - 1 - u[g];
                    a_old[g] = a[h];
                    b_old[g] = b[h];
                    theta_old[g] = theta[h];
                    u_old[g] = u[h];
                    r[g] = k - 1;
                }

            a = a_new;
            b = b_new;

            for (int i = 0; i < e; i++) {
                modint d = 0;
                for (int x = 0; x < a[i].size() && x <= k; x++) d += a[i][x] * S[k - x];
                if (!normalize(i, d)) continue;

                int g = e - 1 - u[i];
                if (!L(a[g], b[g])) {
                    if (b_new[i].size() < k + 1) b_new[i].resize(k + 1);
                    b_new[i][k] += d;
                } else {
                    auto c = theta[i] * modint::inv(theta_old[g]);
                    for (int pow = u[i] - u_old[g]; pow; pow--) c *= pf;

                    if (a_new[i].size() < a_old[g].size() + k - r[g]) a_new[i].resize(a_old[g].size() + k - r[g]);
                    for (int x = 0; x < a_old[g].size(); x++) a_new[i][x + k - r[g]] -= c * a_old[g][x];
                    while (!a_new[i].empty() && !a_new[i].back()) a_new[i].pop_back();

                    if (b_new[i].size() < b_old[g].size() + k - r[g]) b_new[i].resize(b_old[g].size() + k - r[g]);
                    for (int x = 0; x < b_old[g].size(); x++) b_new[i][x + k - r[g]] -= c * b_old[g][x];
                    while (!b_new[i].empty() && !b_new[i].back()) b_new[i].pop_back();
                }
            }
        }

        int d = L(a_new[0], b_new[0]) + 1;
        coeffs.emplace_back(vector<U>(d, 0));
        for (int i = 0; i < a_new[0].size(); i++) coeffs.back()[i] = a_new[0][i]();
        n = max(n, d);
    }
    modint::init(temp);

    vector<modint> A(n - 1);
    for (int i = 1; i < n; i++) {
        T r = 0, lcm = 1;
        for (int j = 0; j < coeffs.size(); j++) {
            T pp = pfs[j].second.first;
            tie(r, lcm) = chinese_remainder_theorem(r, lcm, i < coeffs[j].size() ? (T) coeffs[j][i] % pp : 0, pp);
        }
        A[i - 1] = -r;
    }
    return A;
}

template <typename T>
vector<T> berlekamp_massey(const vector<T> &S) {
    vector<T> B{-1}, C{-1};
    T b = 1;
    for (int n = 1; n <= S.size(); n++) {
        int l = C.size();
        T d = 0;
        for (int i = 0; i < l; i++) d += C[i] * S[n - l + i];
        B.emplace_back(0);
        if (!d) continue;

        int m = B.size();
        T f = d / b;
        if (l < m) {
            auto temp = C;
            C.insert(C.begin(), m - l, 0);
            for (int i = 0; i < m; i++) C[m - 1 - i] -= f * B[m - 1 - i];
            B = temp;
            b = d;
        } else
            for (int i = 0; i < m; i++) C[l - 1 - i] -= f * B[m - 1 - i];
    }
    C.pop_back();
    reverse(C.begin(), C.end());
    return C;
}

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

vector<vector<int>> kmp_automaton(string s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    s += '{';
    vector<vector<int>> fsm(s.size(), vector<int>(26, 0));
    for (int i = 0; i < s.size(); i++)
        for (int c = 0; c < 26; c++)
            fsm[i][c] = (i && 'a' + c != s[i]) ? fsm[pi[i - 1]][c] : i + ('a' + c == s[i]);

    return fsm;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long n, m;
        string p;
        cin >> n >> m >> p;

        modint::init(m);

        int s = p.size();
        auto fsm = kmp_automaton(p);

        vector<modint> dp(s), temp(s), a{1};
        dp[0] = 1;
        while (a.size() < 2 * s) {
            fill(temp.begin(), temp.end(), (modint) 0);
            for (int i = 0; i < s; i++)
                if (dp[i])
                    for (int c = 0; c < 26; c++) {
                        int j = fsm[i][c];
                        if (j < s) temp[j] += dp[i];
                    }
            dp = temp;
            a.emplace_back(accumulate(dp.begin(), dp.end(), (modint) 0));
        }

        if (n < a.size()) {
            cout << modint::pow(26, n) - a[n] << "\n";
            continue;
        }

        auto c = modint::prime_mod ? berlekamp_massey(a) : reeds_sloane(a);
        a.resize(c.size());
        cout << modint::pow(26, n) - kitamasa(c, a, n) << "\n";
    }
}
