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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    modint::init(m);

    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;

        vector<int> d(c);
        for (int &dij : d) cin >> dij;
        sort(d.begin(), d.end());

        for (int l = 0, r; l < c; l = r) {
            for (r = l + 1; r < c && d[l] == d[r]; r++);
            adj_list[i].emplace_back(d[l], r - l);
        }
    }

    vector<int> state(n), order;
    auto dfs = [&](auto &&self, int v = 0) -> bool {
        state[v] = 1;
        for (auto [u, count] : adj_list[v])
            if (!state[u]) {
                if (self(self, u)) return true;
            } else if (state[u] == 1) return true;

        state[v] = 2;
        order.emplace_back(v);
        return false;
    };

    if (dfs(dfs)) {
        cout << "Invalid";
        exit(0);
    }

    int q, t;
    cin >> q >> t;

    if (t) {
        cout << "Valid";
        exit(0);
    }

    vector<modint> cost(n);
    for (int v : order) {
        cost[v] = 1;
        for (auto [u, count] : adj_list[v]) cost[v] += cost[u] * count;
    }

    cout << cost[0] << "\n";
    vector<bitset<1000>> bs1(n);
    for (int i = 0; i < q; i++) {
        int x;
        cin >> x;

        while (x--) {
            int y;
            cin >> y;

            bs1[y][i] = true;
        }
    }

    auto bs2 = bs1;
    vector<modint> sum(q);
    vector<vector<modint>> dp(n, vector<modint>(q));
    for (int v : order) {
        fill(sum.begin(), sum.end(), 0);

        for (auto [u, count] : adj_list[v]) {
            bs2[v] |= bs2[u];
            for (int i = 0; i < q; i++) sum[i] += count * dp[u][i];
        }

        for (int i = 0; i < q; i++)
            if (bs1[v][i]) dp[v][i] = cost[v];
            else if (bs2[v][i]) dp[v][i] = sum[i] + 1;
    }
    for (int i = 0; i < q; i++) cout << dp[0][i] << "\n";
}
