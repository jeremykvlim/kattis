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
        prime_mod = mod() == 998244353 || mod() == (unsigned long long) 1e9 + 7 || mod() == (unsigned long long) 1e9 + 9 || mod() == (unsigned long long) 1e6 + 69 || mod() == 2524775926340780033 || isprime(mod());
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

struct Treap {
    static inline mt19937_64 rng{random_device{}()};
    static inline array<modint, 5> identity;

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        int key;
        array<modint, 5> base, aggregate;

        TreapNode(const int &key = 0, const array<modint, 5> &a = identity) : family{0, 0, 0}, prio(rng()), key(key), base(a), aggregate(a) {}
    };

    int root;
    vector<TreapNode> T;
    stack<int> recycled;

    Treap() : root(0), T(1) {
        identity = {1, 0, 0, 0, 0};
    }

    int node(const int &key, const array<modint, 5> &a) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else {
            T.emplace_back();
            i = T.size() - 1;
        }
        T[i] = TreapNode(key, a);
        return i;
    }

    array<modint, 5> aggregate(int i) {
        return !i ? identity : T[i].aggregate;
    }

    int pull(int i) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        auto add = [&](const array<modint, 5> &a1, const array<modint, 5> &a2) -> array<modint, 5> {
            return {a1[0] * a2[0], a1[1] + a1[0] * a2[1], a2[2] + a2[0] * a1[2], a1[3] + a2[3] + a1[2] * a2[1], a2[0] * a1[4] + a1[0] * a2[4] + a2[2] * a1[1]};
        };
        T[i].aggregate = add(add(aggregate(l), T[i].base), aggregate(r));
        return i;
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, const int &key) {
        if (!i) return {0, 0};
        auto [l, r, p] = T[i].family;
        if (T[i].key > key) {
            auto [ll, lr] = split(l, key);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else {
            auto [rl, rr] = split(r, key);
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        }
    }

    int meld(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }
        if (T[i].prio < T[j].prio) {
            attach(i, 1, meld(T[i].family[1], j));
            T[i].family[2] = 0;
            return i;
        } else {
            attach(j, 0, meld(i, T[j].family[0]));
            T[j].family[2] = 0;
            return j;
        }
    }

    int insert(const int &key, const array<modint, 5> &a) {
        int i = node(key, a);
        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        T[root].family[2] = 0;
        return i;
    }

    int erase_node(int i) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        int m = meld(l, r);
        T[m].family[2] = p;
        recycled.emplace(i);
        (!p ? root : T[p].family[T[p].family[0] != i]) = m;
        for (; p; p = T[p].family[2]) pull(p);
        T[root].family[2] = 0;
        return root;
    }

    array<modint, 5> query() {
        return aggregate(root);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int n, Q;
    cin >> n >> Q;

    Treap treap;
    vector<int> nodes(n);
    vector<modint> b(n), e(n);
    modint base = 0, extra = 0;
    auto update = [&](int i, int a, int x, int p_num, int p_den) {
        modint p = (modint) p_num / p_den, q = 1 - p, p_zero = modint::pow(q, x), p_one = modint::pow(q, x - 1), p_any = 1 - p_zero;
        nodes[i] = treap.insert(a, {p_zero, p_any * a, p_any * a, 0, 0});
        base += b[i] = (p * x + p_zero - 1) * a * a;
        extra += e[i] = ((p_any - p * x * p_one) * a * a) / p_zero;
    };
    for (int i = 0; i < n; i++) {
        int a, x, p_num, p_den;
        cin >> a >> x >> p_num >> p_den;

        update(i, a, x, p_num, p_den);
    }

    auto query = [&]() {
        auto [z, l, r, t, f] = treap.query();
        return base + t + f + z * extra;
    };
    cout << query() << "\n";
    while (Q--) {
        int i, a, x, p_num, p_den;
        cin >> i >> a >> x >> p_num >> p_den;
        i--;

        treap.erase_node(nodes[i]);
        base -= b[i];
        extra -= e[i];
        update(i, a, x, p_num, p_den);
        cout << query() << "\n";
    }
}
