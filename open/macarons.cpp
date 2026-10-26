#include <bits/stdc++.h>
using namespace std;

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

struct DynamicBarrettModInt {
    using H = long long;
    using I = unsigned int;
    using J = unsigned long long;
    using K = unsigned __int128;

    I value;
    static inline I modulus;

    static H mod() {
        return modulus;
    }

    static inline bool prime_mod;
    static inline J inv_mod;
    static constexpr int bit_length = sizeof(J) * 8;

    static void init(I m) {
        modulus = m;
        prime_mod = mod() == 998244353 || mod() == 1e9 + 7 || mod() == 1e9 + 9 || mod() == 1e6 + 69 || isprime(mod());
        inv_mod = (J) -1 / mod();
    }

    constexpr DynamicBarrettModInt() : value() {}

    template <typename V>
    requires numeric_limits<V>::is_integer
    DynamicBarrettModInt(const V &x) {
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

    inline auto & operator+=(const DynamicBarrettModInt &v) {
        H t = (H) value + v.value;
        value = t >= mod() ? t - mod() : t;
        return *this;
    }

    inline auto & operator-=(const DynamicBarrettModInt &v) {
        H t = (H) value - v.value;
        value = t < 0 ? t + mod() : t;
        return *this;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator+=(const V &v) {
        return *this += DynamicBarrettModInt(v);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator-=(const V &v) {
        return *this -= DynamicBarrettModInt(v);
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
        return (DynamicBarrettModInt) 0 - *this;
    }

    auto & operator*=(const DynamicBarrettModInt &v) {
        value = reduce((J) value * v.value);
        return *this;
    }

    auto & operator/=(const DynamicBarrettModInt &v) {
        return *this *= inv(v);
    }

    friend bool operator==(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return lhs.value == rhs.value;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(const DynamicBarrettModInt &lhs, V rhs) {
        return lhs == DynamicBarrettModInt(rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) == rhs;
    }

    friend bool operator!=(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(const DynamicBarrettModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(V lhs, const DynamicBarrettModInt &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator>(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return lhs.value > rhs.value;
    }

    friend bool operator<(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return lhs.value < rhs.value;
    }

    friend DynamicBarrettModInt operator+(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator+(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator+(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) += rhs;
    }

    friend DynamicBarrettModInt operator-(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator-(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator-(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) -= rhs;
    }

    friend DynamicBarrettModInt operator*(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator*(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator*(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) *= rhs;
    }

    friend DynamicBarrettModInt operator/(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator/(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend DynamicBarrettModInt operator/(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) /= rhs;
    }

    template <typename S>
    friend S & operator<<(S &stream, const DynamicBarrettModInt &v) {
        return stream << v();
    }

    template <typename S>
    friend S & operator>>(S &stream, DynamicBarrettModInt &v) {
        H x;
        stream >> x;
        v = DynamicBarrettModInt(x);
        return stream;
    }

    template <typename V>
    static DynamicBarrettModInt pow(DynamicBarrettModInt base, V exponent) {
        DynamicBarrettModInt v = 1;
        while (exponent) {
            if (exponent & 1) v *= base;
            base *= base;
            exponent >>= 1;
        }
        return v;
    }

    static DynamicBarrettModInt inv(const DynamicBarrettModInt &v) {
        if (prime_mod) return pow(v, mod() - 2);

        H x = 0, y = 1, a = v(), m = mod();
        while (a) {
            H t = m / a;
            m -= t * a;
            swap(a, m);
            x -= t * y;
            swap(x, y);
        }

        return (DynamicBarrettModInt) x;
    }
};

using modint = DynamicBarrettModInt;

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init(1e9);

    int n;
    long long m;
    cin >> n >> m;

    int all = 1 << n;
    vector<vector<modint>> adj_matrix(all, vector<modint>(all, 0));
    for (int m0 = 0; m0 < all; m0++) {
        auto dfs = [&](auto &&self, int m1, int m2 = 0, int row = 0) -> void {
            if (row == n) {
                adj_matrix[m0][m2]++;
                return;
            }

            if ((m1 >> row) & 1) self(self, m1, m2, row + 1);
            else {
                self(self, m1 | (1 << row), m2, row + 1);
                if (row + 1 < n && !((m1 >> (row + 1)) & 1)) self(self, m1 | (1 << row) | (1 << (row + 1)), m2, row + 2);
                self(self, m1 | (1 << row), m2 | (1 << row), row + 1);
            }
        };
        dfs(dfs, m0);
    }

    vector<modint> dp(all, 0), temp(all), a{1};
    dp[0] = 1;
    while (a.size() < 2 * all) {
        fill(temp.begin(), temp.end(), 0);
        for (int m1 = 0; m1 < all; m1++)
            if (dp[m1])
                for (int m2 = 0; m2 < all; m2++) temp[m2] += dp[m1] * adj_matrix[m1][m2];
        dp = temp;
        a.emplace_back(dp[0]);
    }

    if (m < a.size()) {
        cout << a[m];
        exit(0);
    }

    auto c = reeds_sloane(a);
    a.resize(c.size());
    cout << kitamasa(c, a, m);
}