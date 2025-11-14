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
struct BarrettModInt {
    using G = int;
    using H = long long;
    using I = unsigned int;
    using J = unsigned long long;
    using K = unsigned __int128;

    I value;
    static inline bool prime_mod;
    static inline J inv_mod;
    static constexpr int bit_length = sizeof(J) * 8;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == 1e9 + 7 || mod() == 1e9 + 9 || mod() == 1e6 + 69 || isprime(mod());
        inv_mod = (J) -1 / mod();
    }

    constexpr BarrettModInt() : value() {}

    template <typename V>
    BarrettModInt(const V &x) {
        value = normalize(x);
    }

    template <typename V>
    static I normalize(const V &x) {
        V v = x;
        if (!(-mod() <= x && x < mod())) v = x % mod();
        return v < 0 ? v + mod() : v;
    }

    static I reduce(J v) {
        H r = (H) (v - (((K) v * inv_mod) >> bit_length) * mod());
        return r >= mod() ? r - mod() : r;
    }

    const I & operator()() const {
        return value;
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    constexpr static H mod() {
        return M::value;
    }

    inline auto & operator+=(const BarrettModInt &v) {
        H t = (H) value + v.value;
        value = t >= mod() ? t - mod() : t;
        return *this;
    }

    inline auto & operator-=(const BarrettModInt &v) {
        H t = (H) value - v.value;
        value = t < 0 ? t + mod() : t;
        return *this;
    }

    template <typename V>
    inline auto & operator+=(const V &v) {
        return *this += BarrettModInt(v);
    }

    template <typename V>
    inline auto & operator-=(const V &v) {
        return *this -= BarrettModInt(v);
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
        return (BarrettModInt) 0 - *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename BarrettModInt<V>::I, unsigned int>::value, BarrettModInt>::type & operator*=(const BarrettModInt &v) {
        value = reduce((J) value * v.value);
        return *this;
    }

    auto & operator/=(const BarrettModInt &v) {
        return *this *= inv(v);
    }

    static BarrettModInt pow(BarrettModInt base, J exponent) {
        BarrettModInt v = 1;
        while (exponent) {
            if (exponent & 1) v *= base;
            base *= base;
            exponent >>= 1;
        }
        return v;
    }

    static BarrettModInt inv(const BarrettModInt &v) {
        if (prime_mod) return pow(v, mod() - 2);

        H x = 0, y = 1, a = v.value, m = mod();
        while (a) {
            H t = m / a;
            m -= t * a;
            swap(a, m);
            x -= t * y;
            swap(x, y);
        }

        return (BarrettModInt) x;
    }
};

template <typename T>
bool operator==(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template <typename T, typename U>
bool operator==(const BarrettModInt<T> &lhs, U rhs) {
    return lhs == BarrettModInt<T>(rhs);
}

template <typename T, typename U>
bool operator==(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) == rhs;
}

template <typename T>
bool operator!=(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(const BarrettModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(U lhs, const BarrettModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator>(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return lhs.value > rhs.value;
}

template <typename T>
bool operator<(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template <typename T>
BarrettModInt<T> operator+(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator+(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator+(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) += rhs;
}

template <typename T>
BarrettModInt<T> operator-(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator-(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator-(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) -= rhs;
}

template <typename T>
BarrettModInt<T> operator*(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator*(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator*(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) *= rhs;
}

template <typename T>
BarrettModInt<T> operator/(const BarrettModInt<T> &lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator/(const BarrettModInt<T> &lhs, U rhs) {
    return BarrettModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
BarrettModInt<T> operator/(U lhs, const BarrettModInt<T> &rhs) {
    return BarrettModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
U & operator<<(U &stream, const BarrettModInt<T> &v) {
    return stream << v();
}

template <typename T, typename U>
U & operator>>(U &stream, BarrettModInt<T> &v) {
    typename make_signed<typename BarrettModInt<T>::I>::type x;
    stream >> x;
    v = BarrettModInt<T>(x);
    return stream;
}

template <typename T>
struct DynamicMod {
    static inline T value;
};

auto &MOD = DynamicMod<unsigned int>::value;
using modint = BarrettModInt<DynamicMod<unsigned int>>;

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
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(m, n);
    if ((b - a) % g) return {0, -1};

    if (m > n) {
        swap(a, b);
        swap(n, m);
    }

    a %= n;
    b %= m;
    T lcm = n / g * m;

    n /= g;
    m /= g;
    auto [x, y] = bezout(n, m);
    T r = ((__int128) a * m * y + (__int128) b * n * x) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}

vector<modint> reeds_sloane(const vector<modint> &S) {
    using U = typename decay<decltype(MOD)>::type;
    using T = make_signed_t<U>;

    int n = 0;
    U undo = MOD;
    auto pfs = factorize(MOD);
    vector<vector<U>> coeffs;
    for (auto &[pf, pows] : pfs) {
        auto [pp, e] = pows;
        MOD = pp;
        modint::init();

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

        coeffs.emplace_back();
        for (auto c : a_new[0]) coeffs.back().emplace_back(c());
        n = max(n, (int) coeffs.back().size());
    }
    MOD = undo;
    modint::init();

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
        vector<T> z(2 * n + 1);
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= n; j++) z[i + j] += x[i] * y[j];

        for (int i = 2 * n; i > n; i--)
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, x0;
    long long n;
    cin >> a >> b >> x0 >> n >> MOD;

    if (MOD == 1) {
        cout << 0;
        exit(0);
    }

    modint::init();

    vector<modint> s{x0};
    for (int i = 1; i < 4; i++) {
        s.emplace_back(s.back());
        s.back() = a * s.back() + b;
    }

    if (n < 4) {
        cout << s[n];
        exit(0);
    }

    auto c = isprime(MOD) ? berlekamp_massey(s) : reeds_sloane(s);
    s.resize(c.size());
    cout << kitamasa(c, s, n);
}
