#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return ((unsigned __int128) x * y) % mod;
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
struct BarrettModInt {
    using I = unsigned int;
    using J = unsigned long long;
    using K = unsigned __int128;

    I value;
    static bool prime_mod;
    static J inv_mod;
    static constexpr int bit_length = sizeof(J) * 8;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == 1e9 + 7 || mod() == 1e9 + 9 || mod() == 1e6 + 69 || isprime(mod());
        inv_mod = (J) -1 / mod() + 1;
    }

    constexpr BarrettModInt() : value() {}

    template <typename V>
    BarrettModInt(const V &x) {
        value = normalize(x);
    }

    template <typename V>
    static I normalize(const V &x) {
        I v = x;
        if (!(-mod() <= x && x < mod())) v = (I) (x % mod());
        return v < 0 ? v + mod() : v;
    }

    static I reduce(J v) {
        v -= (J) (((K) v * inv_mod) >> bit_length) * mod();
        return v >= mod() ? v + mod() : v;
    }

    const I & operator()() const {
        return value;
    }

    template <typename V>
    explicit operator V() const {
        return (V) value;
    }

    constexpr static I mod() {
        return M::value;
    }

    inline auto & operator+=(const BarrettModInt &v) {
        if ((int) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const BarrettModInt &v) {
        if ((int) (value -= v.value) < 0) value += mod();
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

    template <typename V = M>
    typename enable_if<!is_integral<typename BarrettModInt<V>::I>::value, BarrettModInt>::type & operator*=(const BarrettModInt &v) {
        value = reduce(value * v.value);
        return *this;
    }

    auto & operator/=(const BarrettModInt &v) {
        return *this *= inv(v);
    }

    BarrettModInt inv(const BarrettModInt &v) {
        if (prime_mod) {
            BarrettModInt inv = 1, base = v;
            I n = mod() - 2;
            while (n) {
                if (n & 1) inv *= base;
                base *= base;
                n >>= 1;
            }
            return inv;
        }

        I x = 0, y = 1, a = v.value, m = mod();
        while (a) {
            I t = m / a;
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

template <typename U, typename T>
U & operator<<(U &stream, const BarrettModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, BarrettModInt<T> &v) {
    typename common_type<typename BarrettModInt<T>::I, long long>::type x;
    stream >> x;
    v = BarrettModInt<T>(x);
    return stream;
}

template <typename M>
bool BarrettModInt<M>::prime_mod;

template <typename M>
unsigned long long BarrettModInt<M>::inv_mod;

template <typename T>
struct MODULO {
    static T value;
};

template <typename T>
T MODULO<T>::value;

auto &m = MODULO<unsigned int>::value;
using modint = BarrettModInt<MODULO<unsigned int>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n >> m;

    modint::init();

    vector<unordered_map<int, int>> adj_list(n);
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;

        while (c--) {
            int d;
            cin >> d;

            adj_list[i][d]++;
        }
    }

    vector<int> state(n, 0);
    vector<unordered_map<int, int>> build(n);
    auto dfs = [&](auto &&self, int v = 0) -> bool {
        state[v] = 1;
        build[v] = adj_list[v];
        for (auto [u, count] : adj_list[v])
            if (!state[u]) {
                if (self(self, u)) return true;
            }
            else if (state[u] == 1) return true;

        state[v] = 2;
        return false;
    };

    if (dfs(dfs) || build[0].empty()) {
        cout << "Invalid";
        exit(0);
    }

    int q, t;
    cin >> q >> t;

    if (t) {
        cout << "Valid";
        exit(0);
    }

    vector<long long> cost(n, -1);
    auto size = [&](auto &&self, int v = 0) {
        if (cost[v] != -1) return cost[v];

        modint c = 1;
        for (auto [u, count] : build[v]) c += self(self, u) * count;
        return cost[v] = c();
    };

    cout << size(size) << "\n";

    while (q--) {
        int x;
        cin >> x;

        unordered_set<int> y;
        while (x--) {
            int yi;
            cin >> yi;

            y.emplace(yi);
        }

        vector<long long> r(n, -1);
        vector<bool> extra(n, false);
        auto rebuild = [&](auto &&self, int v = 0) {
            if (r[v] != -1) return r[v];

            if (y.count(v)) {
                extra[v] = true;
                return size(size, v);
            }

            modint c = 0;
            for (auto [u, count] : build[v]) {
                c += self(self, u) * count;
                if (extra[u]) extra[v] = true;
            }
            c += extra[v];

            return r[v] = c();
        };

        cout << rebuild(rebuild) << "\n";
    }
}
