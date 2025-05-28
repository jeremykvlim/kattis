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
T inv(T a, T mod) {
    T x = 0, y = 1, m = mod;
    while (a) {
        T t = m / a;
        m -= t * a;
        swap(a, m);
        x -= t * y;
        swap(x, y);
    }

    return (x + mod) % mod;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int &ai : a) {
        cin >> ai;
        ai--;
    }

    vector<bool> seen(n, false);
    vector<vector<vector<int>>> cycles(n + 1);
    for (int i = 0; i < n; i++)
        if (!seen[i]) {
            vector<int> cycle;
            for (int v = i; !seen[v]; v = a[v]) {
                seen[v] = true;
                cycle.emplace_back(v);
            }
            cycles[cycle.size()].emplace_back(cycle);
        }

    auto divs = divisors(k);
    sort(divs.begin(), divs.end());

    vector<int> next(n, -1);
    for (int len = 1; len <= n; len++) {
        int m = cycles[len].size();
        if (!m) continue;

        vector<int> temp;
        for (int d : divs) {
            if (d > m) break;
            if (__gcd(len, k / d) == 1) temp.emplace_back(d);
        }

        if (temp.empty()) {
            cout << "Impossible";
            exit(0);
        }

        vector<int> dp(m + 1, -1);
        dp[0] = 0;
        for (int l = 1; l <= m; l++)
            for (int d : temp)
                if (l >= d && ~dp[l - d]) {
                    dp[l] = d;
                    break;
                }

        if (!~dp[m]) {
            cout << "Impossible";
            exit(0);
        }

        unordered_map<int, int> count;
        while (m > 0) {
            count[dp[m]]++;
            m -= dp[m];
        }

        m = 0;
        for (auto [d, c] : count)
            while (c--) {
                vector<vector<int>> groups;
                for (int _ = 0; _ < d; _++) groups.emplace_back(cycles[len][m++]);

                int l = d * len, K = k % l;
                for (int i = 0; i < d; i++)
                    for (int j = 0; j < len; j++) {
                        int pos = (K * j + i + 1) % l, x = pos % d, y = (((pos - x) / d * inv((k / d) % len, len)) % len);
                        next[groups[i][j]] = groups[x][y];
                    }
            }
    }

    for (int i : next) cout << i + 1 << " ";
}
