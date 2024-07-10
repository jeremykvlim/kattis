#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

const int fixed_point = 9375000;

struct Hash {
    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(int i) const {
        auto key = 0ULL ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
        return h(key);
    }
};

template <typename T>
T mul(T x, T y, T mod) {
    __int128 px = x, py = y, pmod = mod;
    auto product = px * py - pmod * (long long) (1.L / pmod * px * py);
    return product + pmod * (product < 0) - pmod * (product >= pmod);
}

template <typename T>
T pow(T base, T exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

template <typename T>
T lcm(T a, T b) {
    return (a / __gcd(a, b)) * b;
}

template <typename T>
pair<T, T> fib(long long n, T mod = 1) {
    if (!n) return {0, 1};
    else {
        auto [f1, f2] = fib(n >> 1, mod);

        auto fib1 = mul(f1, (2 * f2 - f1 + mod) % mod, mod);
        auto fib2 = (mul(f1, f1, mod) + mul(f2, f2, mod)) % mod;

        if (n & 1) return {fib2, (fib1 + fib2) % mod};
        else return {fib1, fib2};
    }
}

bool isprime(long long n) {
    if (n < 2) return false;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    vector<int> bases{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    int s = __builtin_ctzll(n - 1);
    auto d = n >> s;
    for (int a : bases) {
        auto p = pow(a % n, d, n);
        int i = s;
        while (1 < p && p < n - 1 && a % n && i--) p = mul(p, p, n);
        if (p != n - 1 && i != s) return false;
    }

    return true;
}

long long brent(long long n) {
    if (!(n & 1)) return 2;

    mt19937_64 rng(random_device{}());
    for (;;) {
        auto x = 2LL, y = 2LL, g = 1LL, q = 1LL, xs = -1LL, c = (long long) rng() % (n - 1) + 1;
        for (int i = 1; g == 1; i <<= 1, y = x) {
            for (int j = 1; j < i; j++) x = mul(x, x, n) + c;
            for (int j = 0; j < i && g == 1; j += 128) {
                xs = x;
                for (int k = 0; k < min(128, i - j); k++) {
                    x = mul(x, x, n) + c;
                    q = mul(q, abs(x - y), n);
                }
                g = __gcd(q, n);
            }
        }

        if (g == n) g = 1;
        while (g == 1) {
            xs = mul(xs, xs, n) + c;
            g = __gcd(abs(xs - y), n);
        }
        if (g != n) return g;
    }
}

vector<pair<int, int>> factorize(int n, vector<int> &spf) {
    gp_hash_table<int, int, Hash> pfs;

    auto dfs = [&](auto &&self, int m) -> void {
        if (m < 2) return;
        if (isprime(m) || m < spf.size() && spf[m] == m) {
            pfs[m]++;
            return;
        }

        if (m <= 1e6) {
            while (m > 1) {
                pfs[spf[m]]++;
                m /= spf[m];
            }
            return;
        }

        auto d = brent(m);
        self(self, d);
        self(self, m / d);
    };
    dfs(dfs, n);

    return {pfs.begin(), pfs.end()};
}

void divisors(vector<pair<int, int>> &pfs, vector<int> &divs, long long d = 1, int i = 0) {
    if (i == pfs.size()) return;

    divisors(pfs, divs, d, i + 1);

    auto [pf, pow] = pfs[i];
    while (pow--) {
        d *= pf;
        divs.emplace_back(d);
        divisors(pfs, divs, d, i + 1);
    }
}

int pisano_period(int p, vector<int> &spf, gp_hash_table<int, int, Hash> &cache) {
    if (cache[p]) return cache[p];
    auto pfs = factorize(p, spf);

    auto order = [&](int p) -> int {
        if (p == 2) return 3;
        if (p == 5) return 20;

        auto pfs = factorize((p % 10 == 1 || p % 10 == 9) ? p - 1 : 2 * (p + 1), spf);
        vector<int> divs;
        divisors(pfs, divs);
        sort(divs.begin(), divs.end());
        for (int d : divs)
            if (fib(d, p) == make_pair(0, 1)) return d;
    };

    int pisano = 1;
    for (auto [pf, pow] : pfs) {
        int temp = 1;
        while (pow-- > 1) temp *= pf;
        pisano = lcm(pisano, order(pf) * temp);
    }

    cache[p] = pisano;
    return pisano;
}

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0);
    for (int i = 2; i <= n; i++)
        if (!spf[i])
            for (int j = i; j <= n; j += i)
                if (!spf[j]) spf[j] = i;

    return spf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> F(fixed_point, 0), spf = sieve(1e6);
    F[1] = 1;
    for (int i = 2; i < fixed_point; i++)
        F[i] = (F[i - 1] + F[i - 2]) % fixed_point;

    int t;
    cin >> t;

    gp_hash_table<int, int, Hash> cache, seq_indices, visited;
    gp_hash_table<int, gp_hash_table<long long, pair<int, int>, Hash>, Hash> cycle_members;
    gp_hash_table<int, vector<vector<long long>>, Hash> cycles;
    while (t--) {
        long long n, k;
        int p;
        cin >> n >> k >> p;

        auto g = [&](auto &&self, auto n, auto k, int p) -> long long {
            if (k == 1) return (n >= fixed_point || fixed_point % p) ? fib(n, p).first : F[n] % p;

            int pisano = pisano_period(p, spf, cache);
            if (p != pisano) {
                n = self(self, n, k - 1, pisano);
                return (n >= fixed_point || fixed_point % p) ? fib(n, p).first : F[n] % p;
            }

            n %= p;
            vector<long long> seq{n};
            seq_indices[n] = 0;
            visited[n] = t;
            for (auto i = 1LL; i <= k; i++) {
                n = (n >= fixed_point || fixed_point % p) ? fib(n, p).first : F[n] % p;

                if (cycle_members[p].find(n) != cycle_members[p].end()) {
                    auto [cycle_index, n_index] = cycle_members[p][n];
                    int len = cycles[p][cycle_index].size();
                    i += (k - i) / len * len;
                    return cycles[p][cycle_index][(k - i + n_index) % len];
                }

                if (visited[n] == t) {
                    int len = i - seq_indices[n];
                    vector<long long> cycle(len);
                    for (int j = 0; j < len; j++) {
                        cycle[j] = seq[j + seq_indices[n]];
                        cycle_members[p][seq[j + seq_indices[n]]] = {cycles[p].size(), j};
                    }
                    cycles[p].emplace_back(cycle);
                    i += (k - i) / len * len;
                    return seq[k - i + seq_indices[n]];
                }

                seq.emplace_back(n);
                seq_indices[n] = i;
                visited[n] = t;
            }

            return n;
        };

        cout << g(g, n, k, p) << "\n";
    }
}
