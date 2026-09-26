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
vector<pair<T, int>> factorize(T n) {
    unordered_map<T, int> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs[m]++;
            return;
        }

        T pf = brent(m);
        pfs[pf]++;
        self(self, m / pf);
    };
    dfs(dfs, n);

    return {pfs.begin(), pfs.end()};
}

template <typename T>
vector<T> divisors(T n) {
    auto pfs = factorize(n);
    vector<T> divs{1};

    auto dfs = [&](auto &&self, T d = 1, int i = 0) {
        if (i == pfs.size()) return;

        self(self, d, i + 1);
        auto [pf, pow] = pfs[i];
        while (pow--) {
            d *= pf;
            divs.emplace_back(d);
            self(self, d, i + 1);
        }
    };
    dfs(dfs);

    return divs;
}

struct PURQSegmentTree {
    struct Monoid {
        long long minimum;
        int sum;

        Monoid() : minimum(0), sum(0) {}

        auto & operator=(const int &v) {
            minimum = min(0, v);
            sum = v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            minimum = min(minimum, sum + monoid.minimum);
            sum += monoid.sum;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }
        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n, const vector<int> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> crates(n);
    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        cin >> crates[i];

        pref[i + 1] = pref[i] + crates[i];
    }

    auto total = pref[n];
    vector<int> qs;
    for (auto d : divisors(total))
        if (d >= n) qs.emplace_back(total / d);
    sort(qs.begin(), qs.end());

    auto sorted = crates;
    sort(sorted.begin(), sorted.end());
    vector<long long> pref_sorted(n + 1,0);
    for (int i = 0; i < n; i++) pref_sorted[i + 1] = pref_sorted[i] + sorted[i];

    int index_add = 0, index_sub = 0;
    long long pref_add = 0, pref_sub = 0;
    vector<int> count(total / n + 1, 0);
    vector<pair<int, int>> sweep;
    for (int i = 1; i < n; i++) {
        pref_add += pref[i];
        index_add += i;
        sweep.emplace_back(pref[i] / i + 1, i);
        if (!(pref[i] % i) && pref[i] / i <= total / n) count[pref[i] / i]++;
    }
    sort(sweep.begin(), sweep.end());

    PURQSegmentTree st(n - 1, vector<int>(n - 1, -1));
    auto actions = LLONG_MAX;
    for (int j = 0; int q : qs) {
        for (; j < sweep.size() && sweep[j].first <= q; j++) {
            int i = sweep[j].second;
            pref_sub += pref[i];
            index_sub += i;
            st.point_update(i - 1, 1);
        }
        int pos = upper_bound(sorted.begin(), sorted.end(), q) - sorted.begin();
        auto k = total / q - n, delta = pref_add - (long long) q * index_add - 2 * (pref_sub - (long long) q * index_sub) +
                                        total - pref_sorted[pos] - (long long) q * (n - pos) + count[q];

        int r = n - 1;
        if (!k) {
            for (; r && crates[r] == q; r--);
            if (!r && crates[r] == q) {
                cout << 0;
                exit(0);
            }
            delta -= n - 1 - r;
        }
        auto [minimum, sum] = st.range_query(0, r);
        if (k) minimum = min(minimum, sum - k);
        actions = min(actions, minimum + 2 * delta + q * k * (k + 1));
    }
    cout << actions;
}
