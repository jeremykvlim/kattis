#include <bits/stdc++.h>
using namespace std;

template<typename T>
T inverse(T a, T mod) {
    T u = 0, v = 1;
    while (a) {
        T t = mod / a;
        mod -= t * a;
        swap(a, mod);
        u -= t * v;
        swap(u, v);
    }

    return u;
}

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

bool isprime(long long n) {
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

constexpr int MODULO = 1e9 + 7;
const bool PRIME_MOD = MODULO == 998244353 || MODULO == 1e9 + 7 || MODULO == 1e9 + 9 || MODULO == 1e6 + 69 || isprime(MODULO);

template<typename M>
struct ModInt {
    using T = typename decay<decltype(M::value)>::type;
    T value;

    constexpr ModInt() : value() {}

    template<typename U>
    ModInt(const U &x) {
        value = normalize(x);
    }

    template<typename U>
    static T normalize(const U &x) {
        T v = x;
        if (!(-mod() <= x && x < mod())) v = (T) (x % mod());
        if (v < 0) v += mod();
        return v;
    }

    const T & operator()() const {
        return value;
    }

    template<typename U>
    explicit operator U() const {
        return (U) value;
    }

    constexpr static T mod() {
        return M::value;
    }

    inline auto & operator+=(const ModInt &v) {
        if ((value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const ModInt &v) {
        if ((value -= v.value) < 0) value += mod();
        return *this;
    }

    template<typename U>
    inline auto & operator+=(const U &v) {
        return *this += ModInt(v);
    }

    template<typename U>
    inline auto & operator-=(const U &v) {
        return *this -= ModInt(v);
    }

    auto & operator++() {
        return *this += 1;
    }

    auto & operator--() {
        return *this -= 1;
    }

    auto operator++(int) {
        ModInt v(*this);
        *this += 1;
        return v;
    }

    auto operator--(int) {
        ModInt v(*this);
        *this -= 1;
        return v;
    }

    auto operator-() const {
        return ModInt(-value);
    }

    template<typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, int>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize((long long) value * (long long) v.value);
        return *this;
    }

    template<typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, long long>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(value * v.value - (long long) ((long double) value * v.value / mod()) * mod());
        return *this;
    }

    template<typename U = M>
    typename enable_if<!is_integral<typename ModInt<U>::T>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(value * v.value);
        return *this;
    }

    auto & operator/=(const ModInt &v) {
        if (PRIME_MOD) return *this *= ModInt(pow(v.value, mod() - 2, mod()));
        return *this *= ModInt(inverse(v.value, mod()));
    }
};

template<typename T>
bool operator==(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template<typename T, typename U>
bool operator==(const ModInt<T> &lhs, U rhs) {
    return lhs == ModInt<T>(rhs);
}

template<typename T, typename U>
bool operator==(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) == rhs;
}

template<typename T>
bool operator!=(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator!=(const ModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator!=(U lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator<(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template<typename T>
ModInt<T> operator+(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template<typename T, typename U>
ModInt<T> operator+(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) += rhs;
}

template<typename T, typename U>
ModInt<T> operator+(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template<typename T>
ModInt<T> operator-(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template<typename T, typename U>
ModInt<T> operator-(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template<typename T, typename U>
ModInt<T> operator-(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template<typename T>
ModInt<T> operator*(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template<typename T, typename U>
ModInt<T> operator*(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template<typename T, typename U>
ModInt<T> operator*(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template<typename T>
ModInt<T> operator/(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template<typename T, typename U>
ModInt<T> operator/(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template<typename T, typename U>
ModInt<T> operator/(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename U, typename T>
U & operator<<(U &stream, const ModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, ModInt<T> &v) {
    typename common_type<typename ModInt<T>::T, long long>::type x;
    stream >> x;
    v.value = ModInt<T>::normalize(x);
    return stream;
}

using modint = ModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    friend auto operator *(Matrix<T> &A, Matrix<T> &B) {
        int r1 = A.r, r2 = B.r, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < r2; k++) C[i][j] += A[i][k] * B[k][j];

        return C;
    }

    friend auto operator *=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
Matrix<T> I(int size) {
    Matrix<T> I(size);
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

template <typename T>
Matrix<T> matpow(Matrix<T> a, int exponent) {
    int size = a.r;
    auto b = I<T>(size);

    for (; exponent; exponent >>= 1) {
        if (exponent & 1) b = a * b;
        a = a * a;
    }

    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, k;
        cin >> n >> k;

        Matrix<modint> a1(50), b1(50), a2(50, 1), b2(50, 1);
        for (int i = 1; i < 50; i++) a1[i][i - 1] = b1[i][i - 1] = 1;
        a2[0][0] = b2[0][0] = 1;

        while (k--) {
            int l;
            cin >> l;

            a1[0][l - 1]++;
            b1[0][l - 1] = 1;
        }

        a1 = matpow(a1, n);
        b1 = matpow(b1, n);
        a2 = a1 * a2;
        b2 = b1 * b2;

        if (b2[0][0]) cout << a2[0][0] << "\n";
        else cout << "IMPOSSIBLE\n";
    }
}
