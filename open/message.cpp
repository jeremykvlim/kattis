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
        if (exponent & 1) value = mul(value, base, mod);
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
        int s = __builtin_ctzll(n - 1);
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
struct ModInt {
    using T = typename decay<decltype(M::value)>::type;

    T value;
    static bool prime_mod;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == (unsigned long long) 1e9 + 7 || mod() == (unsigned long long) 1e9 + 9 || mod() == (unsigned long long) 1e6 + 69 || mod() == 2524775926340780033 || isprime(mod());
    }

    constexpr ModInt() : value() {}

    template <typename U>
    ModInt(const U &x) {
        value = normalize(x);
    }

    template <typename U>
    static T normalize(const U &x) {
        U v = x;
        if (!(-mod() <= x && x < mod())) v %= mod();
        return v < 0 ? v + mod() : v;
    }

    const T & operator()() const {
        return value;
    }

    template <typename U>
    explicit operator U() const {
        return (U) value;
    }

    constexpr static T mod() {
        return M::value;
    }

    inline auto & operator+=(const ModInt &v) {
        if ((long long) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const ModInt &v) {
        if ((long long) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename U>
    inline auto & operator+=(const U &v) {
        return *this += ModInt(v);
    }

    template <typename U>
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
        return *this += 1;
    }

    auto operator--(int) {
        return *this -= 1;
    }

    auto operator-() const {
        return (ModInt) 0 - *this;
    }

    template <typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, unsigned int>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize((unsigned long long) value * v.value);
        return *this;
    }

    template <typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, unsigned long long>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(mul(value, v.value, mod()));
        return *this;
    }

    template <typename U = M>
    typename enable_if<!is_integral<typename ModInt<U>::T>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(value * v.value);
        return *this;
    }

    auto & operator/=(const ModInt &v) {
        return *this *= inv(v);
    }

    ModInt inv(const ModInt &v) {
        if (prime_mod) {
            ModInt inv = 1, base = v;
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

        return (ModInt) x;
    }
};

template <typename T>
bool operator==(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template <typename T, typename U>
bool operator==(const ModInt<T> &lhs, U rhs) {
    return lhs == ModInt<T>(rhs);
}

template <typename T, typename U>
bool operator==(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) == rhs;
}

template <typename T>
bool operator!=(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(const ModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(U lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator>(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value > rhs.value;
}

template <typename T>
bool operator<(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template <typename T>
ModInt<T> operator+(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
ModInt<T> operator+(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
ModInt<T> operator+(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T>
ModInt<T> operator-(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
ModInt<T> operator-(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
ModInt<T> operator-(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T>
ModInt<T> operator*(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
ModInt<T> operator*(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
ModInt<T> operator*(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T>
ModInt<T> operator/(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
ModInt<T> operator/(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
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
    v = ModInt<T>(x);
    return stream;
}

template <typename M>
bool ModInt<M>::prime_mod;

template <typename T>
struct MODULO {
    static T value;
};

template <typename T>
T MODULO<T>::value;

auto &m = MODULO<unsigned long long>::value;
using modint = ModInt<MODULO<unsigned long long>>;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    friend auto operator*(Matrix<T> &A, Matrix<T> &B) {
        int r1 = A.r, r2 = B.r, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < r2; k++) C[i][j] += A[i][k] * B[k][j];

        return C;
    }

    auto & operator[](int i) {
        return mat[i];
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
        A = A * A;
        exponent >>= 1;
    }
    return B;
}

vector<int> prefix_function(string s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

vector<vector<int>> automaton(string s, vector<int> pi) {
    vector<vector<int>> fsm(s.size(), vector<int>(26));
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
        long long n;
        string p;
        cin >> n >> m >> p;

        modint::init();

        int s = p.size();
        auto pi = prefix_function(p);
        auto fsm = automaton(p, pi);

        Matrix<modint> count(s);
        for (int i = 0; i < s; i++)
            for (int c = 0; c < 26; c++)
                if (fsm[i][c] < s) count[i][fsm[i][c]]++;

        count = matpow(count, n);
        cout << pow(26ULL, n, m) - accumulate(count[0].begin(), count[0].end(), (modint) 0) << "\n";
    }
}
