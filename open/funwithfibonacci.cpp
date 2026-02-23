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

template <typename T, typename U>
pair<T, T> fib(U n, T mod = 1) {
    if (!n) return {0, 1};
    else {
        auto [f1, f2] = fib(n >> 1, mod);

        auto fib1 = mul(f1, (2 * f2 - f1 + mod) % mod, mod);
        auto fib2 = (mul(f1, f1, mod) + mul(f2, f2, mod)) % mod;

        if (n & 1) return {fib2, (fib1 + fib2) % mod};
        else return {fib1, fib2};
    }
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
    for (int i = 0; i < 9; i++) {
        int p = A235702[i];
        pos[i].assign(p, -1);
        visited[i].assign(p, -1);
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
                    int temp = 1;
                    while (pow-- > 1) temp *= pf;

                    auto prime_period = [&]() -> int {
                        if (pf == 2) return 3;
                        if (pf == 5) return 20;
                        if (pf % 10 == 1 || pf % 10 == 9) return pf - 1;
                        return 2 * (pf + 1);
                    };
                    pisano = lcm(pisano, prime_period() * temp);
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
