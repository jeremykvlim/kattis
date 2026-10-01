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
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    inline auto & operator+=(const V &v) {
        return *this += DynamicBarrettModInt(v);
    }

    template <typename V>
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    friend bool operator==(const DynamicBarrettModInt &lhs, V rhs) {
        return lhs == DynamicBarrettModInt(rhs);
    }

    template <typename V>
    requires is_integral_v<V>
    friend bool operator==(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) == rhs;
    }

    friend bool operator!=(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires is_integral_v<V>
    friend bool operator!=(const DynamicBarrettModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires is_integral_v<V>
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
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator+(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) += rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator+(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) += rhs;
    }

    friend DynamicBarrettModInt operator-(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator-(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) -= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator-(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) -= rhs;
    }

    friend DynamicBarrettModInt operator*(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator*(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) *= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator*(V lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) *= rhs;
    }

    friend DynamicBarrettModInt operator/(const DynamicBarrettModInt &lhs, const DynamicBarrettModInt &rhs) {
        return DynamicBarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
    friend DynamicBarrettModInt operator/(const DynamicBarrettModInt &lhs, V rhs) {
        return DynamicBarrettModInt(lhs) /= rhs;
    }

    template <typename V>
    requires is_integral_v<V>
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

    int n, M, k, p;
    cin >> n >> M >> k >> p;

    modint::init(p);

    vector<vector<int>> adj_list(n + 1);
    while (M--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    modint count = 1;
    vector<int> state(n + 1, 0), color(n + 1, 0);
    vector<vector<int>> children(n + 1);
    vector<vector<modint>> dp(n + 1, vector<modint>(5));
    for (int root = 1; root <= n; root++)
        if (!state[root]) {
            vector<int> cyclic;
            auto dfs1 = [&](auto &&self, int v, int prev = -1) -> void {
                state[v] = 1;
                for (int u : adj_list[v])
                    if (u != prev) {
                        if (!state[u]) {
                            children[v].emplace_back(u);
                            self(self, u, v);
                        } else if (state[u] == 1 && state[v] == 1) {
                            state[v] = 2;
                            cyclic.emplace_back(v);
                        }
                    }
            };
            dfs1(dfs1, root);

            modint sum = 0;
            auto assign = [&](auto &&self, int depth = 0, int j = 0) -> void {
                auto dfs2 = [&](auto &&self, int v) {
                    if (j > k) return;

                    fill(dp[v].begin(), dp[v].end(), 0);
                    if (state[v] != 2)
                        for (int i = 0; i <= j; i++) dp[v][i] = 1;
                    else dp[v][color[v]] = 1;

                    for (int u : adj_list[v])
                        if (state[u] == 2) dp[v][color[u]] = 0;

                    for (int u : children[v]) {
                        self(self, u);
                        for (int i = 0; i <= j; i++) dp[v][i] *= dp[u][j + 1] - dp[u][i];
                    }

                    for (int i = 0; i < j; i++) dp[v][j + 1] += dp[v][i];
                    dp[v][j + 1] += dp[v][j] * (k - j);

                    if (v == root) {
                        for (int i = k; i > k - j; i--) dp[v][j + 1] *= i;
                        sum += dp[v][j + 1];
                    }
                };

                if (depth < cyclic.size())
                    for (int i = 0; i <= j; i++) {
                        color[cyclic[depth]] = i;
                        self(self, depth + 1, max(i + 1, j));
                    }
                else dfs2(dfs2, root);
            };
            assign(assign);
            count *= sum;
        }

    cout << count;
}