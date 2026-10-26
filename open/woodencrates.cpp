#include <bits/stdc++.h>
using namespace std;

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
vector<pair<T, int>> factorize(T n) {
    unordered_map<T, int> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs[m]++;
            return;
        }

        T d = pollard_brent(m);
        self(self, d);
        self(self, m / d);
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
