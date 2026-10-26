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

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    T q = a / b, r = a - q * b;
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(m, n);
    if ((b - a) % g) return {0, -1};

    T n0 = n / g, m0 = m / g, lcm = n0 * m;
    auto [_, x, y] = extended_gcd(n0, m0);
    T r = ((__int128) n * (((__int128) ((b - a) / g) * x % m0 + m0) % m0) + a) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long e;
    cin >> n >> e;

    if (n == 1) {
        cout << "infinity";
        exit(0);
    }

    n *= 2;
    auto divs = divisors(n);

    auto sum = [&](long long x) { return x * (x - 1) / 2; };

    auto time = 0LL;
    auto add = [&](int t) {
        if (e < t) {
            cout << time + e * e - sum(e);
            exit(0);
        }
        time += e * t - sum(t);
        e -= t - 2;
    };

    vector<int> seen(n, -1), unique_t;
    for (int r = 0;;) {
        int rem = e % n;

        if (seen[rem] != -1) {
            int l = seen[rem];

            int delta = 0;
            for (int i = l; i < r; i++) delta += unique_t[i] - 2;
            if (delta <= 0) {
                cout << "infinity";
                exit(0);
            }

            int cycle_len = r - l;
            vector<int> cycle_t(cycle_len);
            for (int i = l; i < r; i++) cycle_t[i - l] = unique_t[i];

            long long used = 0, cycle_delta = 0, extra = 0;
            for (int i = 0; i < cycle_len; i++) {
                used = max(used, cycle_t[i] + cycle_delta);
                extra += cycle_t[i] * cycle_delta + sum(cycle_t[i]);
                cycle_delta += cycle_t[i] - 2;
            }

            if (e > used) {
                auto k = (e - used) / delta;
                time += (k * e - sum(k) * delta) * accumulate(cycle_t.begin(), cycle_t.end(), 0) - k * extra;
                e -= k * delta;
            }
            for (int t : cycle_t) add(t);
        } else {
            seen[rem] = r;
            int t = INT_MAX, b = (2 * rem + 1) % n;
            for (int d : divs) {
                int m = n / d;
                auto [x, lcm] = chinese_remainder_theorem(0, d, b % m, m);
                if (lcm != -1) t = min(t, !x ? lcm : x);
            }
            add(t);
            unique_t.emplace_back(t);
            r++;
        }
    }
}
