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
    typename common_type<typename BarrettModInt<T>::I, long long>::type x;
    stream >> x;
    v = BarrettModInt<T>(x);
    return stream;
}

constexpr unsigned int MODULO = 5318008;
using modint = BarrettModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    vector<vector<modint>> trinomial(5e3, vector<modint>(5e3, 0));
    trinomial[0][0] = 1;
    for (int i = 1; i < 5e3; i++) {
        trinomial[i][0] = trinomial[i - 1][0] + 2 * trinomial[i - 1][1];
        for (int j = 1; j <= i; j++) trinomial[i][j] = trinomial[i - 1][j - 1] + trinomial[i - 1][j] + trinomial[i - 1][j + 1];
    }

    int t;
    cin >> t;

    while (t--) {
        int n, x1, y1, x2, y2;
        cin >> n >> x1 >> y1 >> x2 >> y2;

        if (abs(x1 - x2) < abs(y1 - y2)) {
            swap(x1, y1);
            swap(x2, y2);
        }

        int x = abs(x1 - x2), Y1 = abs(y1 - y2), Y2 = y1 + y2, Y3 = 2 * n - y1 - y2 + 2;
        cout << trinomial[x][Y1] - (Y2 < 5e3 ? trinomial[x][Y2] : 0) - (Y3 < 5e3 ? trinomial[x][Y3] : 0) << "\n";
    }
}
