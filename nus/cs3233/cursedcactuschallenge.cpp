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
struct MontgomeryModInt {
    using T = typename decay<decltype(M::value)>::type;
    using U = typename conditional<is_same<T, unsigned int>::value, unsigned long long, typename conditional<is_same<T, unsigned long long>::value, unsigned __int128, void>::type>::type;
    using I = typename conditional<is_same<T, unsigned int>::value, int, typename conditional<is_same<T, unsigned long long>::value, long long, void>::type>::type;
    using J = typename conditional<is_same<T, unsigned int>::value, long long, typename conditional<is_same<T, unsigned long long>::value, __int128, void>::type>::type;

    T value;
    static inline pair<T, U> r;
    static inline bool prime_mod;
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
        return (MontgomeryModInt) 0 - *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned int>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((unsigned long long) value * v.value);
        return *this;
    }

    template <typename V = M>
    typename enable_if<is_same<typename MontgomeryModInt<V>::T, unsigned long long>::value, MontgomeryModInt>::type & operator*=(const MontgomeryModInt &v) {
        value = reduce((unsigned __int128) value * v.value);
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
    typename common_type<typename MontgomeryModInt<T>::T, long long>::type x;
    stream >> x;
    v = MontgomeryModInt<T>(x);
    return stream;
}

constexpr unsigned long long MODULO = 998244353;
using modint = MontgomeryModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

pair<vector<bool>, vector<vector<int>>> tarjan(int n, int m, vector<vector<pair<int, int>>> &adj_list, vector<pair<int, int>> &edges) {
    vector<int> order(n + 1, 0), low(n + 1, 0);
    vector<bool> cycle_edge(m + 1, false);
    vector<vector<int>> cycles;
    stack<int> st;
    int count = 0;

    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        for (auto [u, i] : adj_list[v])
            if (u != prev) {
                if (!order[u]) {
                    st.emplace(i);
                    self(self, u, v);
                    low[v] = min(low[v], low[u]);

                    if (low[u] >= order[v]) {
                        vector<int> component;
                        int j;
                        do {
                            j = st.top();
                            st.pop();

                            component.emplace_back(j);
                        } while (j != i);

                        if (component.size() > 1) {
                            unordered_map<int, vector<int>> temp;
                            for (int k : component) {
                                cycle_edge[k] = true;
                                auto [x, y] = edges[k];
                                temp[x].emplace_back(y);
                                temp[y].emplace_back(x);
                            }

                            int p = edges[component[0]].first;
                            vector<int> cycle{p};
                            int q = -1, s = p;
                            do {
                                int t = (temp[s][0] == q ? temp[s][1] : temp[s][0]);
                                q = s;
                                s = t;
                                cycle.emplace_back(s);
                            } while (s != p);
                            cycle.pop_back();
                            cycles.emplace_back(cycle);
                        }
                    }
                } else {
                    if (order[u] < order[v]) st.emplace(i);
                    low[v] = min(low[v], order[u]);
                }
            }
    };
    dfs(dfs);

    return {cycle_edge, cycles};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int n, m;
    cin >> n >> m;

    vector<modint> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<pair<int, int>> edges(m + 1);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 1; i <= m; i++) {
        auto &[u, v] = edges[i];
        cin >> u >> v;

        adj_list[u].emplace_back(v, i);
        adj_list[v].emplace_back(u, i);
    }

    auto [cycle_edge, cycles] = tarjan(n, m, adj_list, edges);
    int k = n + cycles.size();
    vector<vector<int>> adj_list_tree(k + 1);
    for (int i = 1; i <= m; i++)
        if (!cycle_edge[i]) {
            auto [u, v] = edges[i];
            adj_list_tree[u].emplace_back(v);
            adj_list_tree[v].emplace_back(u);
        }

    for (int c = 0; c < cycles.size(); c++) {
        int u = c + n + 1;
        for (int v : cycles[c]) {
            adj_list_tree[u].emplace_back(v);
            adj_list_tree[v].emplace_back(u);
        }
    }

    vector<array<modint, 3>> dp1(k + 1), dp2(k + 1);
    auto add = [&](const auto &s1, const auto &s2) -> array<modint, 3> {
        return {s1[0] + s2[0], s1[1] + s2[1], s1[2] + s2[2]};
    };

    auto mul = [&](const auto &s1, const auto &s2) -> array<modint, 3> {
        return {s1[0] * s2[0], s1[1] * s2[0] + s1[0] * s2[1], s1[2] * s2[0] + s1[0] * s2[2] + 2 * s1[1] * s2[1]};
    };

    auto compute = [&](const auto &chain, bool force) -> array<modint, 3> {
        if (chain.empty()) return {1, 0, 0};

        auto s1 = dp1[chain[0]], s2 = force ? array<modint, 3>{0, 0, 0} : dp2[chain[0]];
        for (int i = 1; i < chain.size(); i++) {
            auto t1 = mul(add(s1, s2), dp1[chain[i]]), t2 = mul(s1, dp2[chain[i]]);
            s1 = t1;
            s2 = t2;
        }

        if (!force) s1 = add(s1, s2);
        return s1;
    };

    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        for (int u : adj_list_tree[v])
            if (u != prev) self(self, u, v);

        if (v <= n) {
            dp1[v] = {1, 0, 0};
            dp2[v] = {1, a[v], a[v] * a[v]};
            for (int u : adj_list_tree[v])
                if (u != prev) {
                    if (u <= n) {
                        dp1[v] = mul(dp1[v], add(dp1[u], dp2[u]));
                        dp2[v] = mul(dp2[v], dp1[u]);
                    } else {
                        dp1[v] = mul(dp1[v], dp1[u]);
                        dp2[v] = mul(dp2[v], dp2[u]);
                    }
                }
        } else {
            int c = v - n - 1, s = cycles[c].size(), j = 0;
            for (int i = 0; i < s; i++)
                if (cycles[c][i] == prev) {
                    j = i;
                    break;
                }

            vector<int> chain;
            for (int i = 1; i < s; i++) chain.emplace_back(cycles[c][(i + j) % s]);
            dp1[v] = compute(chain, false);
            dp2[v] = compute(chain, true);
        }
    };
    dfs(dfs);

    cout << add(dp1[1], dp2[1])[2];
}
