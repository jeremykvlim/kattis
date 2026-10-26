#include <bits/stdc++.h>
using namespace std;

istream & operator>>(istream &stream, __int128 &x) {
    string s;
    stream >> s;

    x = 0;
    for (int sgn = s[0] == '-' ? -1 : 1, i = sgn < 0; i < s.size(); i++) x = x * 10 + sgn * (s[i] - '0');
    return stream;
}

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

    auto inv = n, one = -n % n;
    while (n * inv != 1) inv *= 2 - n * inv;

    auto add = [&](auto x, auto y) {
        return x >= n - y ? x - (n - y) : x + y;
    };

    auto sub = [&](auto x, auto y) {
        return x >= y ? x - y : n - (y - x);
    };

    auto mul = [&](auto x, auto y) -> unsigned long long {
        auto z = (unsigned __int128) x * y;
        unsigned long long q = z * inv;
        auto v = (z >> 64) + n - (((unsigned __int128) q * n) >> 64);
        return v >= n ? v - n : v;
    };

    auto pow = [&](auto base, auto exponent) {
        auto value = one;
        while (exponent) {
            if (exponent & 1) value = mul(value, base);
            base = mul(base, base);
            exponent >>= 1;
        }
        return value;
    };

    auto two = add(one, one);
    auto miller_rabin = [&](auto a) {
        int s = countr_zero(n - 1);
        auto x = pow(a, (n - 1) >> s);
        if (x == one || x == n - one) return true;

        while (--s) {
            x = mul(x, x);
            if (x == n - one) return true;
        }
        return false;
    };
    if (!miller_rabin(two) || !miller_rabin(add(two, one))) return false;

    auto P = 3ULL;
    for (;; P++) {
        auto D = P * P - 4;
        auto jacobi_symbol = [&](auto n) {
            int j = 1;
            auto a = D;
            while (a) {
                for (; !(a & 1); a >>= 1)
                    if ((n & 7) == 3 || (n & 7) == 5) j = -j;
                if ((a & 3) == 3 && (n & 3) == 3) j = -j;

                swap(a, n);
                a %= n;
            }
            return n == 1 ? j : 0;
        };
        int j = jacobi_symbol(n);
        if (j == -1) break;
        if (!j && D % n) return false;
    }
    P = ((unsigned __int128) P * one % n);

    int s = countr_zero(n + 1);
    auto k = (n + 1) >> s, V_k = P, V_kp1 = sub(mul(P, P), two);
    for (int i = bit_width(k) - 2; i >= 0; i--) {
        auto V_2kp1 = sub(mul(V_k, V_kp1), P);

        if ((k >> i) & 1) {
            V_k = V_2kp1;
            V_kp1 = sub(mul(V_kp1, V_kp1), two);
        } else {
            V_kp1 = V_2kp1;
            V_k = sub(mul(V_k, V_k), two);
        }
    }
    if ((V_k == two && V_kp1 == P) || (V_k == sub(0, two) && V_kp1 == sub(0, P))) return true;

    while (--s) {
        if (!V_k) return true;
        V_k = sub(mul(V_k, V_k), two);
    }
    return false;
}

unsigned long long pollard_brent(unsigned long long n) {
    if (!(n & 1)) return 2;

    auto inv = n, one = -n % n;
    while (n * inv != 1) inv *= 2 - n * inv;

    auto add = [&](auto x, auto y) {
        return x >= n - y ? x - (n - y) : x + y;
    };

    auto mul = [&](auto x, auto y) -> unsigned long long {
        auto z = (unsigned __int128) x * y;
        unsigned long long q = z * inv;
        auto v = (z >> 64) + n - (((unsigned __int128) q * n) >> 64);
        return v >= n ? v - n : v;
    };

    static mt19937_64 rng(random_device{}());
    for (;;) {
        auto x = add(one, one), y = x, z = one, xs = x, d = 1ULL, c = rng() % (n - 1) + 1;

        auto f = [&](auto x) {
            return add(mul(x, x), c);
        };

        for (int i = 1; d == 1; i <<= 1, y = x) {
            for (int _ = 0; _ < i - 1; _++) x = f(x);
            for (int j = 0; j < i && d == 1; j += 300) {
                xs = x;
                for (int k = 0; k < min(300, i - j); k++) {
                    x = f(x);
                    z = mul(z, max(x, y) - min(x, y));
                }
                d = __gcd(z, n);
            }
        }

        if (d == n) d = 1;
        while (d == 1) {
            xs = f(xs);
            d = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (d != n) return d;
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

        T d = pollard_brent(m);
        self(self, d);
        self(self, m / d);
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

    T phi = !(m & 1) ? m / 2 / pfs[1] * (pfs[1] - 1) : m / pfs[0] * (pfs[0] - 1);
    pfs = factorize(phi);
    sort(pfs.begin(), pfs.end());
    pfs.erase(unique(pfs.begin(), pfs.end()), pfs.end());
    for (auto g = 2LL; g < m; g++)
        if (gcd(g, m) == 1 && all_of(pfs.begin(), pfs.end(), [&](auto pf) { return pow((T) g, phi / pf, m) != 1; })) return g;

    return m;
}

template <auto M>
struct MontgomeryModInt {
    using T = conditional_t<__lg(M) < 30, unsigned int, unsigned long long>;
    using U = conditional_t<is_same_v<T, unsigned int>, unsigned long long, unsigned __int128>;
    using I = conditional_t<is_same_v<T, unsigned int>, int, long long>;
    using J = conditional_t<is_same_v<T, unsigned int>, long long, __int128>;

    T value;

    constexpr static T mod() {
        return M;
    }

    static constexpr int p2 = countr_zero((T) (mod() - 1));
    static inline pair<T, U> r = [] {
        pair<T, U> r{(T) M, -(U) M % (T) M};
        while ((T) M * r.first != 1) r.first *= (T) 2 - (T) M * r.first;
        return r;
    }();
    static constexpr int bit_length = sizeof(T) * 8;
    static inline bool prime_mod = mod() == 998244353 || mod() == 1000000007 || mod() == 1000000009 || mod() == 1000069 || mod() == 2524775926340780033 || mod() == 39582418599937 || mod() == 79164837199873 || isprime(mod());

    constexpr MontgomeryModInt() : value() {}

    MontgomeryModInt(const J &x) {
        J v = x % mod();
        if (v < 0) v += mod();
        value = reduce((U) v * r.second);
    }

    template <auto N>
    MontgomeryModInt(const MontgomeryModInt<N> &x) {
        value = reduce((U) (x() % mod()) * r.second);
    }

    static T reduce(const U &x) {
        T q = x * r.first, v = (x >> bit_length) + mod() - (((U) q * mod()) >> bit_length);
        return v >= mod() ? v - mod() : v;
    }

    T operator()() const {
        return reduce((U) value);
    }

    template <typename V>
    explicit operator V() const {
        return (V) (*this)();
    }

    I recover() const {
        T v = reduce((U) value);
        return v > mod() / 2 ? v - mod() : v;
    }

    static T primitive_root() {
        static const T g = primitive_root_mod_m(mod());
        return g;
    }

    static bool ntt_viable(int n) {
        return prime_mod && !(n & (n - 1)) && __lg(n) <= p2;
    }

    inline auto & operator+=(const MontgomeryModInt &v) {
        if ((I) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const MontgomeryModInt &v) {
        if ((I) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator+=(const V &v) {
        return *this += (MontgomeryModInt) v;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    inline auto & operator-=(const V &v) {
        return *this -= (MontgomeryModInt) v;
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
        return (MontgomeryModInt) 0 - *this;
    }

    auto & operator*=(const MontgomeryModInt &v) {
        if constexpr (is_same_v<T, unsigned int>) value = reduce((unsigned long long) value * v.value);
        else value = reduce((unsigned __int128) value * v.value);
        return *this;
    }

    auto & operator/=(const MontgomeryModInt &v) {
        return *this *= inv(v);
    }

    friend bool operator==(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs.value == rhs.value;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(const MontgomeryModInt &lhs, V rhs) {
        return lhs == MontgomeryModInt(rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator==(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) == rhs;
    }

    friend bool operator!=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(const MontgomeryModInt &lhs, V rhs) {
        return !(lhs == rhs);
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend bool operator!=(V lhs, const MontgomeryModInt &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator>(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs() > rhs();
    }

    friend bool operator<(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs() < rhs();
    }

    friend bool operator>=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs > rhs || lhs == rhs;
    }

    friend bool operator<=(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return lhs < rhs || lhs == rhs;
    }

    friend MontgomeryModInt operator+(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator+(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator+(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) += rhs;
    }

    friend MontgomeryModInt operator-(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator-(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator-(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) -= rhs;
    }

    friend MontgomeryModInt operator*(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator*(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator*(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) *= rhs;
    }

    friend MontgomeryModInt operator/(const MontgomeryModInt &lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator/(const MontgomeryModInt &lhs, V rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename V>
    requires numeric_limits<V>::is_integer
    friend MontgomeryModInt operator/(V lhs, const MontgomeryModInt &rhs) {
        return MontgomeryModInt(lhs) /= rhs;
    }

    template <typename S>
    friend S & operator<<(S &stream, const MontgomeryModInt &v) {
        return stream << v();
    }

    template <typename S>
    friend S & operator>>(S &stream, MontgomeryModInt &v) {
        J x;
        stream >> x;
        v = MontgomeryModInt(x);
        return stream;
    }

    template <typename V>
    static MontgomeryModInt pow(MontgomeryModInt base, V exponent) {
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

        J x = 0, y = 1;
        T a = v(), m = mod();
        while (a) {
            T t = m / a;
            m -= t * a;
            swap(a, m);
            x -= (J) t * y;
            swap(x, y);
        }

        return (MontgomeryModInt) x;
    }
};

constexpr int MOD = 998244353;
using modint = MontgomeryModInt<MOD>;

struct SuffixArray {
    vector<int> SA, ascii, SA_inv;

    vector<int> sais(vector<int> &ascii1, int range) {
        int n = ascii1.size();
        if (!n) return {};
        if (n == 1) return {0};
        if (n == 2) return ascii1[0] < ascii1[1] ? vector<int>{0, 1} : vector<int>{1, 0};

        vector<int> sa(n, 0), sum_s(range + 1, 0), sum_l(range + 1, 0);
        vector<bool> sl(n, false);
        for (int i = n - 2; ~i; i--) sl[i] = ascii1[i] == ascii1[i + 1] ? sl[i + 1] : ascii1[i] < ascii1[i + 1];
        for (int i = 0; i < n; i++)
            if (!sl[i]) sum_s[ascii1[i]]++;
            else sum_l[ascii1[i] + 1]++;

        for (int i = 0; i <= range; i++) {
            sum_s[i] += sum_l[i];
            if (i < range) sum_l[i + 1] += sum_s[i];
        }

        auto induced_sort = [&](vector<int> &lms) {
            fill(sa.begin(), sa.end(), -1);
            vector<int> b(range + 1, 0);
            copy(sum_s.begin(), sum_s.end(), b.begin());
            for (int i : lms) sa[b[ascii1[i]]++] = i;

            copy(sum_l.begin(), sum_l.end(), b.begin());
            sa[b[ascii1[n - 1]]++] = n - 1;
            for (int j : sa)
                if (j > 0 && !sl[j - 1]) sa[b[ascii1[j - 1]]++] = j - 1;

            copy(sum_l.begin(), sum_l.end(), b.begin());
            for (int i = n - 1; ~i; i--) {
                int j = sa[i];
                if (j > 0 && sl[j - 1]) sa[--b[ascii1[j - 1] + 1]] = j - 1;
            }
        };

        vector<int> lms_map(n + 1, -1), lms;
        int m = 0;
        for (int i = 1; i < n; i++)
            if (!sl[i - 1] && sl[i]) {
                lms_map[i] = m++;
                lms.emplace_back(i);
            }
        induced_sort(lms);

        if (m) {
            vector<int> lms_sorted, ascii2(m);
            for (int j : sa)
                if (lms_map[j] != -1) lms_sorted.emplace_back(j);

            int range2 = 0;
            ascii2[lms_map[lms_sorted[0]]] = 0;
            for (int i = 1; i < m; i++) {
                int l = lms_sorted[i - 1], r = lms_sorted[i], l_end = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n, r_end = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
                bool same = true;
                if (l_end - l != r_end - r) same = false;
                else {
                    for (; l < l_end && ascii1[l] == ascii1[r]; l++, r++);
                    if (l == n || ascii1[l] != ascii1[r]) same = false;
                }

                if (!same) range2++;
                ascii2[lms_map[lms_sorted[i]]] = range2;
            }

            auto sa2 = sais(ascii2, range2);
            for (int i = 0; i < m; i++) lms_sorted[i] = lms[sa2[i]];
            induced_sort(lms_sorted);
        }

        return sa;
    }

    vector<int> kasai() {
        int n = ascii.size();
        vector<int> lcp(n);
        SA_inv.resize(n);
        for (int i = 0; i < n; i++) SA_inv[SA[i]] = i;
        for (int i = 0, k = 0; i < n; i++) {
            if (k) k--;
            if (!SA_inv[i]) continue;

            int j = SA[SA_inv[i] - 1];
            while (i + k < n && j + k < n && ascii[i + k] == ascii[j + k]) k++;
            lcp[SA_inv[i] - 1] = k;
        }
        lcp.back() = n;
        return lcp;
    }

    int & operator[](int i) {
        return SA[i];
    }

    SuffixArray(string &s, int r = 128) : ascii(s.begin(), s.end()) {
        SA = sais(ascii, r);
    };
};

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    cin >> c;

    while (c--) {
        string s;
        cin >> s;

        int n = s.size();

        if (n == 1) {
            cout << "1\n";
            continue;
        }

        SuffixArray sa(s);
        vector<int> lcp = sa.kasai(), indices(n - 1), len(n);
        iota(indices.begin(), indices.end(), 0);
        sort(indices.begin(), indices.end(), [&](int i, int j) { return lcp[i] > lcp[j]; });
        for (int i = 0; i < n; i++) len[i] = n - i;

        DisjointSets dsu(n);
        modint h = (long long) n * n * *min_element(lcp.begin(), lcp.end());

        auto compute = [&](auto &&self, int i = 0) -> void {
            if (i >= n - 1) return;

            auto add = [&](int i, int l) {
                int i_set = dsu.find(i), s = dsu.size(i_set);
                h += (long long) s * s * (len[i_set] - l);
            };

            int l = lcp[indices[i]];
            for (; i < n - 1 && lcp[indices[i]] == l; i++) {
                int curr = sa[indices[i] + 1], prev = sa[indices[i]];
                add(curr, l);
                add(prev, l);

                auto [big, small] = dsu.unite(curr, prev);
                if (small == -1) len[big] = min(len[big], l);
                else len[big] = min({len[big], len[small], l});
            }

            self(self, i);
        };
        compute(compute);

        cout << h << "\n";
    }
}
