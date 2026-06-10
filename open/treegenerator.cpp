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
    typename make_signed<typename BarrettModInt<T>::I>::type x;
    stream >> x;
    v = BarrettModInt<T>(x);
    return stream;
}

template <typename T>
struct DynamicMod {
    static inline T value;
};

auto &MOD = DynamicMod<unsigned int>::value;
using modint = BarrettModInt<DynamicMod<unsigned int>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> MOD >> q;

    modint::init();

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int x, y;
        cin >> x >> y;

        adj_list[x].emplace_back(y);
        adj_list[y].emplace_back(x);
    }

    vector<int> digit(n + 1);
    for (int i = 1; i <= n; i++) cin >> digit[i];

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> child(n + 1, 0), depth(n + 1, 0), inlabel(n + 1), ascendant(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v, int prev = 0) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs, n);
    for (auto [v, p] : tour) {
        ascendant[v] = ascendant[p] | lsb(inlabel[v]);
        if (v != n && inlabel[v] == inlabel[p]) child[p] = v;
    }

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    vector<int> base, offset(n + 1), pos(n + 1);
    for (auto [v, p] : tour)
        if (v == n || inlabel[v] != inlabel[p]) {
            offset[inlabel[v]] = base.size();
            for (int u = v; u; u = child[u]) {
                base.emplace_back(digit[u]);
                pos[u] = base.size() - 1;
            }
        }

    vector<modint> p10(n + 1, 1), pref1(n + 1), pref2(n + 1);
    for (int i = 0; i < n; i++) {
        p10[i + 1] = p10[i] * 10;
        pref1[i + 1] = pref1[i] * 10 + base[i];
        pref2[i + 1] = pref2[i] * 10 + base[n - 1 - i];
    }

    auto concat = [&](auto x, auto y) -> pair<modint, int> {
        return {x.first * p10[y.second] + y.first, x.second + y.second};
    };

    auto calc = [&](const auto &pref, int l, int r) -> pair<modint, int> {
        return {pref[r] - pref[l] * p10[r - l], r - l};
    };

    auto path_up = [&](int v, int a) {
        pair<modint, int> val{0, 0};
        for (; inlabel[v] != inlabel[a]; v = head[inlabel[v]]) val = concat(val, calc(pref2, n - 1 - pos[v], n - offset[inlabel[v]]));
        return concat(val, calc(pref2, n - 1 - pos[v], n - pos[a]));
    };

    auto path_down = [&](int v, int a) {
        pair<modint, int> val{0, 0};
        for (; inlabel[v] != inlabel[a]; v = head[inlabel[v]]) val = concat(calc(pref1, offset[inlabel[v]], pos[v] + 1), val);
        return concat(calc(pref1, pos[a] + 1, pos[v] + 1), val);
    };

    while (q--) {
        int a, b;
        cin >> a >> b;

        int c = lca(a, b);
        cout << concat(path_up(a, c), path_down(b, c)).first << "\n";
    }
}
