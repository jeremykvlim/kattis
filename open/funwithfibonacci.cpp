#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
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

template <typename T, typename U>
pair<T, T> fib(U n, T mod = 1) {
    if (!n) return {0, 1 % mod};

    auto add = [&](T x, T y) {
        return x >= mod - y ? x - (mod - y) : x + y;
    };

    auto sub = [&](T x, T y) {
        return x >= y ? x - y : mod - (y - x);
    };

    auto [f1, f2] = fib(n >> 1, mod);
    T fib1 = mul(f1, sub(add(f2, f2), f1), mod), fib2 = add(mul(f1, f1, mod), mul(f2, f2, mod));
    if (n & 1) return {fib2, add(fib1, fib2)};
    return {fib1, fib2};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> A235702{24, 120, 600, 3000, 15000, 75000, 375000, 1875000, 9375000};
    int fixed_point = A235702.back();
    vector<int> memo(fixed_point, 0), cache(fixed_point + 1, 0);
    memo[1] = 1;
    for (int i = 2; i < fixed_point; i++) memo[i] = (memo[i - 1] + memo[i - 2]) % fixed_point;

    vector<vector<int>> pos(9), visited(9);
    vector<vector<array<int, 3>>> cycles(9);
    for (int p5 = 0; p5 < 9; p5++) {
        int p = A235702[p5];
        pos[p5].assign(p, -1);
        visited[p5].assign(p, -1);
    }

    int t;
    cin >> t;

    while (t--) {
        long long n, k;
        int p;
        cin >> n >> k >> p;

        auto g = [&](auto &&self, long long k, long long n, int p) -> int {
            if (p == 1) return 0;
            if (k == 1) return fib(n, p).first;

            auto pisano_period = [&]() {
                if (cache[p]) return cache[p];

                int pisano = 1;
                for (auto [pf, pow] : factorize(p)) {
                    int pp = 1;
                    while (pow-- > 1) pp *= pf;

                    auto prime_period = [&]() -> int {
                        if (pf == 2) return 3;
                        if (pf == 5) return 20;
                        if (pf % 10 == 1 || pf % 10 == 9) return pf - 1;
                        return 2 * (pf + 1);
                    };
                    pisano = lcm(pisano, prime_period() * pp);
                }
                return cache[p] = pisano;
            };
            int pisano = pisano_period();
            if (pisano != p) return fib(self(self, k - 1, n, pisano), p).first;

            int temp = n %= p, p5 = 0;
            for (pisano /= 24; !(pisano % 5); pisano /= 5, p5++);

            int f = -1, j = 0, s = cycles[p5].size();
            for (; !~visited[p5][n]; j++) {
                pos[p5][n] = j + s;
                visited[p5][n] = t;
                n = memo[n] % p;
                if (!--k) f = n;
            }
            int r = j + s;

            cycles[p5].resize(r);
            int l = pos[p5][n];
            if (visited[p5][n] == t) {
                j -= r - l;
                for (int i = l; i < r; i++) {
                    cycles[p5][i] = {(int) n, l | (1 << 30), r - l};
                    n = memo[n] % p;
                }
            }

            n = temp;
            for (int i = 0; i < j; i++) {
                cycles[p5][i + s] = {(int) n, l, j - 1 - i};
                n = memo[n] % p;
            }
            if (k <= 0) return f;

            for (; !((cycles[p5][l][1] >> 30) & 1); l = cycles[p5][l][1]) {
                int len = cycles[p5][l][2];
                if (k <= len) return cycles[p5][l + k][0];
                k -= len + 1;
            }
            int i = cycles[p5][l][1] & ((1 << 30) - 1);
            return cycles[p5][i + ((l - i + k) % cycles[p5][l][2])][0];
        };
        cout << g(g, k, n, p) << "\n";
    }
}
