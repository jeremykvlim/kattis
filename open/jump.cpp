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

    mt19937_64 rng(random_device{}());
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

        auto pf = brent(m);
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

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
pair<T, T> chinese_remainder_theorem(T a, T n, T b, T m) {
    T g = __gcd(n, m);
    if ((b - a) % g) return {0, -1};

    if (m > n) {
        swap(a, b);
        swap(n, m);
    }

    a %= n;
    b %= m;
    T lcm = n / g * m;

    n /= g;
    m /= g;
    auto [x, y] = bezout(n, m);
    T r = ((__int128) a * m * y + (__int128) b * n * x) % lcm;
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
