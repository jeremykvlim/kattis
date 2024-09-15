#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

template <typename T>
T mul(T x, T y, T mod) {
    long long px = x, py = y, pmod = mod;
    auto product = px * py - pmod * (long long) (1.L / pmod * px * py);
    return product + pmod * (product < 0) - pmod * (product >= pmod);
}

template <typename T>
T pow(T base, T exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

bool isprime(unsigned long long n) {
    if (n < 2) return false;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    vector<int> bases{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    int s = __builtin_ctzll(n - 1);
    auto d = n >> s;
    for (int a : bases) {
        auto p = pow(a % n, d, n);
        int i = s;
        while (1 < p && p < n - 1 && a % n && i--) p = mul(p, p, n);
        if (p != n - 1 && i != s) return false;
    }

    return true;
}

template <typename M>
struct MontgomeryModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, unsigned __int128, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;
    using J = typename conditional<is_same<T, unsigned int>::value, long long, typename conditional<is_same<T, unsigned long long>::value, __int128, void>::type>::type;

    T value;
    static pair<T, U> r;
    static bool prime_mod;
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
        return (MontgomeryModInt) -value;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned int>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((unsigned long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned long long>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((__uint128_t) value * v.value);
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

    MontgomeryModInt inv(const MontgomeryModInt &v) {
        if (prime_mod) {
            MontgomeryModInt inv = 1, base = v;
            T n = mod() - 2;
            while (n) {
                if (n & 1) inv *= base;
                base *= base;
                n >>= 1;
            }
            return inv;
        }

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

template <typename U, typename T>
U & operator<<(U &stream, const MontgomeryModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, MontgomeryModInt<T> &v) {
    typename common_type<typename MontgomeryModInt<T>::T, long long>::type x;
    stream >> x;
    v = MontgomeryModInt<T>(x);
    return stream;
}

template <typename M>
pair<typename MontgomeryModInt<M>::T, typename MontgomeryModInt<M>::U> MontgomeryModInt<M>::r;

template <typename M>
bool MontgomeryModInt<M>::prime_mod;

constexpr unsigned int MODULO = 3;
using modint = MontgomeryModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

struct Hash {
    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const vector<modint> &v) const {
        auto key = 0ULL;
        for (auto e : v) key ^= e() + 0x9e3779b9 + (key << 6) + (key >> 2);
        return h(key);
    }
};

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
void rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabs(matrix[i][c]) > fabs(matrix[pivot][c])) pivot = i;

        if (fabs(matrix[pivot][c]) < 1e-9) continue;
        swap(matrix[pivot], matrix[rank]);

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabs(matrix[i][c]) > 1e-9) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int n, k, t;
    cin >> n >> k >> t;

    int t_max = pow(3, k, 0);

    vector<vector<int>> tritsets(t_max, vector<int>(k));
    vector<int> cw, ccw;
    for (int i = 0; i < t_max; i++) {
        int temp = i, first_set_trit = -1;
        for (int j = 0; j < k; j++) {
            int trit = temp % 3;
            if (first_set_trit == -1 && trit) first_set_trit = trit;

            tritsets[i][j] = trit;
            temp /= 3;
        }

        if (first_set_trit == 1) cw.emplace_back(i);
    }
    ccw.resize(cw.size(), 0);
    for (int i = 0; i < cw.size(); i++) {
        auto rev_ts = tritsets[cw[i]];
        for (int &trit : rev_ts) trit = (3 - trit) % 3;

        reverse(rev_ts.begin(), rev_ts.end());
        for (int trit : rev_ts) ccw[i] = 3 * ccw[i] + trit;
    }

    unordered_map<char, int> ctoi{{'A', 0}, {'E', 1}, {'I', 2}};
    unordered_map<int, char> itoc{{0, '0'}, {1, '+'}, {2, '-'}};

    auto read = [&](string s) {
        int encoded = 0;
        reverse(s.begin(), s.end());
        for (char c : s) encoded = 3 * encoded + ctoi[c];
        return encoded;
    };

    auto write = [&](int i) {
        for (int trit : tritsets[i]) cout << itoc[trit];
        cout << "\n";
    };

    vector<long long> f(t_max, 0);
    vector<int> ids;
    while (t--) {
        string s;
        cin >> s;

        ids.emplace_back(read(s));
        cin >> f[ids.back()];
    }

    vector<complex<long long>> F(t_max, 0), w{{1, 0}, {0, 1}, {-1, -1}};
    for (int i = 0; i < t_max; i++)
        for (int j : ids) {
            modint dot = 0;
            for (int l = 0; l < k; l++) dot += tritsets[i][l] * tritsets[j][l];

            F[i] += f[j] * w[dot()];
        }

    vector<int> dir(t_max), tzs(t_max);
    for (int i = 0; i < t_max; i++)
        if (!F[i].imag()) {
            dir[i] = 0;
            tzs[i] = __builtin_ctzll(abs(F[i].real()));
        } else if (F[i].real() == F[i].imag()) {
            dir[i] = 1;
            tzs[i] = __builtin_ctzll(abs(F[i].real()));
        } else {
            dir[i] = 2;
            tzs[i] = __builtin_ctzll(abs(F[i].imag()));
        }

    int h = (t_max - 1) / 2, z = (3 * accumulate(tzs.begin(), tzs.end(), 0) - t_max * n) / (2 * t_max);
    n -= z;
    for (int &tz : tzs) tz -= z;

    vector<array<int, 2>> levers;
    vector<array<vector<int>, 3>> indices(t_max);
    if (k == 1) {
        for (int i = 0; i < h; i++)
            for (int j = 0; j < h; j++) {
                modint dot = 0;
                for (int l = 0; l < k; l++) dot += tritsets[cw[i]][l] * tritsets[cw[j]][l];

                indices[cw[i]][dot()].emplace_back(j);
                indices[ccw[i]][(3 - dot)()].emplace_back(j);
            }

        for (int i = 0; i < tzs[0]; i++) levers.push_back({1, 2});
    } else {
        Matrix<double> A(h, h + 1);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < h; j++) {
                modint dot = 0;
                for (int l = 0; l < k; l++) dot += tritsets[cw[i]][l] * tritsets[cw[j]][l];

                indices[cw[i]][dot()].emplace_back(j);
                indices[ccw[i]][(3 - dot)()].emplace_back(j);

                if (!dot()) A[i][j] = 1;
            }

            A[i][h] = tzs[cw[i]];
        }
        rref(A);

        for (int i = 0; i < h; i++) {
            if (A[i][h] < 1e-9) continue;
            while ((int) round(A[i][h]--)) levers.push_back({cw[i], ccw[i]});
        }
    }

    int m = n / 2;
    gp_hash_table<vector<modint>, int, Hash> cache;
    vector<modint> before(h, 0), after(h);
    for (int i = 0; i < h; i++) after[i] = dir[cw[i]];

    auto backtrack1 = [&](auto &&self, int depth, int v = 0) {
        if (depth == -1) {
            cache[before] = v;
            return;
        }

        for (int c = 0; c < 2; c++) {
            for (int trit = 1; trit < 3; trit++)
                for (int i : indices[levers[depth][c]][trit]) before[i] += trit;
            self(self, depth - 1, v << 1 | c);
            for (int trit = 1; trit < 3; trit++)
                for (int i : indices[levers[depth][c]][trit]) before[i] -= trit;
        }
    };

    auto backtrack2 = [&](auto self, int depth, int v = 0) {
        if (depth == m - 1) {
            if (cache.find(after) == cache.end()) return;

            int u = cache[after];
            for (int i = 0; i < z; i++) write(0);
            for (int i = 0; i < m; i++) write(levers[i][(u >> i) & 1]);
            for (int i = m; i < n; i++) write(levers[i][(v >> (i - m)) & 1]);
            exit(0);
        }

        for (int c = 0; c < 2; c++) {
            for (int trit = 1; trit < 3; trit++)
                for (int i : indices[levers[depth][c]][trit]) after[i] += 3 - trit;
            self(self, depth - 1, v << 1 | c);
            for (int trit = 1; trit < 3; trit++)
                for (int i : indices[levers[depth][c]][trit]) after[i] -= 3 - trit;
        }
    };

    backtrack1(backtrack1, m - 1);
    backtrack2(backtrack2, n - 1);
}
