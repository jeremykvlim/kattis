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
    requires is_integral_v<V>
    inline auto & operator+=(const V &v) {
        return *this += (MontgomeryModInt) v;
    }

    template <typename V>
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    friend bool operator==(const MontgomeryModInt &lhs, V rhs) {
        return lhs == MontgomeryModInt(rhs);
    }

    template <typename V>
    requires is_integral_v<V>
    friend bool operator==(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) == rhs;
    }

    friend bool operator!=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires is_integral_v<V>
    friend bool operator!=(const MontgomeryModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    friend MontgomeryModInt operator+(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend MontgomeryModInt operator+(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    friend MontgomeryModInt operator-(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend MontgomeryModInt operator-(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend MontgomeryModInt operator-(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    friend MontgomeryModInt operator*(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend MontgomeryModInt operator*(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend MontgomeryModInt operator*(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    friend MontgomeryModInt operator/(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend MontgomeryModInt operator/(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
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

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
T rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    T det = 1;
    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (matrix[i][c] > matrix[pivot][c]) pivot = i;

        if (!(matrix[pivot][c])) continue;
        swap(matrix[pivot], matrix[rank]);
        if (pivot != rank) det *= -1;

        det *= matrix[rank][c];
        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    return rank < n ? 0 : det;
}

template <typename T>
T kirchoffs_theorem(int n, const vector<pair<int, int>> &edges) {
    vector<int> degree(n, 0);
    for (auto [u, v] : edges) {
        degree[u]++;
        degree[v]++;
    }

    Matrix<T> laplacian(n - 1);
    for (int i = 0; i < n - 1; i++) laplacian[i][i] = degree[i];
    for (auto [u, v] : edges)
        if (u != n - 1 && v != n - 1) {
            laplacian[u][v]--;
            laplacian[v][u]--;
        }

    return rref(laplacian);
}

struct DisjointSets {
    int t;
    vector<int> sets, seen;

    int find(int v) {
        if (seen[v] != t) {
            seen[v] = t;
            sets[v] = -1;
            return v;
        }

        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    void reset() {
        t++;
    }

    DisjointSets(int n) : t(1), sets(n, -1), seen(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int v, e;
    cin >> v >> e;

    if (v - 1 == e) {
        cout << 0;
        exit(0);
    }

    vector<vector<bool>> adj_matrix(v, vector<bool>(v, false));
    DisjointSets dsu(v);
    int components = v;
    while (e--) {
        int a, b;
        cin >> a >> b;
        a--; b--;

        adj_matrix[a][b] = adj_matrix[b][a] = true;
        if (dsu.unite(a, b)) components--;
    }

    if (components != 1) {
        cout << 0;
        exit(0);
    }

    vector<vector<modint>> dp(1 << v, vector<modint>(v, 0));
    for (int i = 0; i < v; i++) dp[1 << i][i] = 1;

    for (int mask = 1; mask < 1 << v; mask++) {
        int j = has_single_bit((unsigned) (mask & -mask)) ? __lg(mask & -mask) : 0;
        for (int i = j; i < v; i++)
            if ((mask >> i) & 1)
                for (int k = j + 1; k < v; k++)
                    if (adj_matrix[i][k] && !((mask >> k) & 1)) dp[mask | (1 << k)][k] += dp[mask][i];
    }

    modint count = 0;
    for (int mask = 1; mask < 1 << v; mask++) {
        modint c = 0;
        int j = has_single_bit((unsigned) (mask & -mask)) ? __lg(mask & -mask) : 0;
        for (int i = j + 1; i < v; i++)
            if (adj_matrix[i][j] && (mask >> i) & 1 && (mask != ((1 << i) | (1 << j)))) c += dp[mask][i];
        if (!c) continue;
        c /= 2;

        dsu.reset();
        for (int i = 0; i < v; i++)
            if ((mask >> i) & 1) dsu.unite(i, j);

        vector<int> indices(v, -1);
        int n = 0;
        for (int i = 0; i < v; i++)
            if (dsu.find(i) == i) indices[i] = n++;

        if (n <= 1) {
            count += c;
            continue;
        }

        vector<pair<int, int>> edges;
        for (int i = 0; i < v; i++) {
            int i_set = dsu.find(i);
            for (int k = i + 1; k < v; k++)
                if (adj_matrix[i][k]) {
                    int k_set = dsu.find(k);
                    if (i_set != k_set) edges.emplace_back(indices[i_set], indices[k_set]);
                }
        }

        count += c * kirchoffs_theorem<modint>(n, edges);
    }
    cout << count;
}
