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
        prime_mod = mod() == 998244353 || mod() == (unsigned long long) 1e9 + 7 || mod() == (unsigned long long) 1e9 + 9 || mod() == (unsigned long long) 1e6 + 69 || mod() == 2524775926340780033 || mod() == 39582418599937 || mod() == 79164837199873 || isprime(mod());
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
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    friend auto operator*(const Matrix<T> &A, const Matrix<T> &B) {
        int r1 = A.r, c1 = A.c, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int k = 0; k < c1; k++)
                if (A[i][k])
                    for (int j = 0; j < c2; j++) C[i][j] += A[i][k] * B[k][j];
        return C;
    }

    friend auto operator*(const Matrix<T> &A, const vector<T> &v) {
        int n = A.r, m = v.size();

        vector<T> u(n, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) u[i] += A[i][j] * v[j];

        return u;
    }

    friend auto operator*=(Matrix<T> &A, const Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }

    Matrix<T> transpose() const {
        Matrix<T> mat_T(c, r);

        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) mat_T[j][i] = mat[i][j];

        return mat_T;
    }
};

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n);
    for (int i = 0; i < n; i++) I[i][i] = 1;
    return I;
}

template <typename T, typename U>
Matrix<T> matpow(Matrix<T> A, U exponent) {
    int n = A.r;
    auto B = I<T>(n);

    while (exponent) {
        if (exponent & 1) B = A * B;
        A *= A;
        exponent >>= 1;
    }
    return B;
}

struct PURQSegmentTree {
    static inline vector<int> a, sgn;
    static inline int n;
    static inline vector<Matrix<modint>> ways;

    struct Monoid {
        static inline int p;
        static inline array<vector<modint>, 4> U, V;

        Matrix<modint> M;

        Monoid() : M(I<modint>(25)) {}
        Monoid(Monoid &&) = default;

        Monoid(const Monoid &monoid) {
            if (p <= 1) M = monoid.M;
            else if (p & 1)
                for (int i = 0; i < 4; i++) U[i] = monoid.M * U[i];
        }

        auto & operator=(const Monoid &monoid) {
            if (p > 1) {
                p >>= 1;
                for (int k = 0; k < 4; k++)
                    for (int i = 0; i < 25; i++)
                        if (U[k][i])
                            for (int j = 0; j < 25; j++) M[i][j] += U[k][i] * V[k][j];
            } else M = monoid.M;

            return *this;
        }

        auto & operator=(int v) {
            if (p < 0) {
                if (v >= 2 * a.size() - 1) M = I<modint>(25);
                else {
                    M = Matrix<modint>(25);
                    if (v & 1)
                        for (int i = 0; i < 5; i++)
                            for (int f1 = 0; f1 < 5; f1++)
                                for (int f2 = 0; f2 < 5; f2++) M[i * 5 + f1][i * 5 + f2] = ways[v >> 1][f1][f2];
                    else
                        for (int i = 0; i < 5; i++)
                            for (int f = 0; f < 5; f++) M[i * 5 + f][min(i + (a[v >> 1] == f), 4) * 5 + f] = 1;
                }

                return *this;
            }

            for (int i = 0; i < 4; i++) {
                fill(U[i].begin(), U[i].end(), 0);
                fill(V[i].begin(), V[i].end(), 0);

                int j = 5 * i + a[v >> 1];
                U[i][j] = sgn[v >> 1];
                V[i][j] = -1;
                V[i][j + 5] = 1;
                M[j][j] -= sgn[v >> 1];
                M[j][j + 5] += sgn[v >> 1];
            }
            sgn[v >> 1] *= -1;

            p = v + n;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (p > 1) {
                if (!(p & 1)) {
                    auto M_T = monoid.M.transpose();
                    for (int i = 0; i < 4; i++) V[i] = M_T * V[i];
                }

                return *this;
            }

            M *= monoid.M;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int m, const vector<int> &arr, const vector<Matrix<modint>> &w) : ST(2 * m) {
        a = arr;
        sgn.assign(arr.size(), -1);
        n = m;
        ways = w;

        for (int i = 0; i < 4; i++) {
            Monoid::U[i].resize(25);
            Monoid::V[i].resize(25);
        }
        Monoid::p = -1;
        for (int i = 0; i < m; i++) ST[i + m] = i;
        Monoid::p = 0;
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int n, m, p, Q;
    cin >> n >> m >> p >> Q;

    vector<int> positions{1, n};
    vector<array<int, 4>> sweep(2 * m);
    for (int i = 0; i < m; i++) {
        int c, d, a, b;
        cin >> c >> d >> a >> b;

        positions.emplace_back(c);
        positions.emplace_back(d);
        sweep[2 * i] = {c, a, b, 1};
        sweep[2 * i + 1] = {d, a, b, -1};
    }
    sort(sweep.begin(), sweep.end());

    vector<int> e(p), f(p);
    for (int i = 0; i < p; i++) {
        cin >> e[i] >> f[i];

        positions.emplace_back(e[i]);
    }
    sort(positions.begin(), positions.end());
    positions.erase(unique(positions.begin(), positions.end()), positions.end());

    Matrix<modint> temp(5);
    vector<Matrix<modint>> ways(p - 1, I<modint>(5));
    vector<vector<int>> delta(5, vector<int>(5, 0));
    for (int k = 0, i = 0, j = 0; k + 1 < positions.size(); k++) {
        for (; i < sweep.size() && sweep[i][0] == positions[k]; i++) delta[sweep[i][1]][sweep[i][2]] += sweep[i][3];
        for (; j + 1 < p && e[j + 1] <= positions[k]; j++);
        if (j + 1 < p && e[j] <= positions[k] && e[j + 1] >= positions[k + 1]) {
            for (int r = 0; r < 5; r++)
                for (int c = 0; c < 5; c++) temp[r][c] = !delta[r][c];
            ways[j] *= matpow(temp, positions[k + 1] - positions[k]);
        }
    }

    PURQSegmentTree st(bit_ceil((unsigned) 2 * p - 1), f, ways);
    while (Q--) {
        int q;
        cin >> q;

        if (!q) {
            int g, h, k;
            cin >> g >> h >> k;

            auto A = st.range_query(2 * (g - 1), 2 * (h - 1) + 1).M;
            modint sum = 0;
            for (int f1 = 0; f1 < 5; f1++)
                for (int i = k; i < 5; i++)
                    for (int f2 = 0; f2 < 5; f2++) sum += A[f1][i * 5 + f2];
            cout << sum << "\n";
        } else {
            int l;
            cin >> l;

            st.point_update(2 * (l - 1), 2 * (l - 1));
        }
    }
}
