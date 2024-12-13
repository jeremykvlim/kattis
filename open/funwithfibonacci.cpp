#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

const int fixed_point = 9375000;

struct Hash {
    size_t operator()(const int &i) const {
        return 0ULL ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
    }
};

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
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
        int s = __builtin_ctzll(n - 1);
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
    gp_hash_table<T, int, Hash> pfs;

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

int pisano_period(int p, gp_hash_table<int, int, Hash> &cache) {
    if (cache[p]) return cache[p];

    auto pfs = factorize(p);

    auto prime_period = [&](int p) -> int {
        if (p == 2) return 3;
        if (p == 5) return 20;

        auto divs = divisors((p % 10 == 1 || p % 10 == 9) ? p - 1 : 2 * (p + 1));
        sort(divs.begin(), divs.end());
        for (int d : divs)
            if (fib(d, p) == make_pair(0, 1)) return d;
    };

    int pisano = 1;
    for (auto [pf, pow] : pfs) {
        int temp = 1;
        while (pow-- > 1) temp *= pf;
        pisano = lcm(pisano, prime_period(pf) * temp);
    }

    return cache[p] = pisano;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> memo(fixed_point, 0);
    memo[1] = 1;
    for (int i = 2; i < fixed_point; i++)
        memo[i] = (memo[i - 1] + memo[i - 2]) % fixed_point;

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
            auto F = [&](int n) -> long long {
                return (n >= fixed_point || fixed_point % p) ? fib(n, p).first : memo[n] % p;
            };

            if (k == 1) return F(n);

            int pisano = pisano_period(p, cache);
            if (p != pisano) return F(self(self, n, k - 1, pisano));

            n %= p;
            vector<long long> seq{n};
            seq_indices[n] = 0;
            visited[n] = t;
            for (auto i = 1LL; i <= k; i++) {
                n = F(n);

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
