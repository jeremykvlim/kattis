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
vector<T> factorize(T n) {
    vector<T> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs.emplace_back(m);
            return;
        }

        T pf = brent(m);
        pfs.emplace_back(pf);
        self(self, m / pf);
    };
    dfs(dfs, n);

    return pfs;
}

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

    long long n, k;
    cin >> n >> k;

    auto g = gcd(n, k);
    if (g > 1) {
        if (k > 1e5) cout << "Hellingur!";
        else
            for (int i = 1; i <= k / g; i++)
                for (int j = 0; j < g; j++) cout << i + (n / g) * j << " ";
        exit(0);
    }

    auto pfs = factorize(n);
    sort(pfs.begin(), pfs.end());
    pfs.erase(unique(pfs.begin(), pfs.end()), pfs.end());
    int m = pfs.size();
    if (m == 1) {
        cout << "O nei!";
        exit(0);
    }

    auto check = [&](auto K) {
        if (pfs.size() == 2) return mul(K % pfs[1], pow(pfs[0], pfs[1] - 2, pfs[1]), pfs[1]) <= K / pfs[0];
        vector<long long> dist(pfs[0], 1e18);
        dist[0] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        pq.emplace(0, 0);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (int i = 1; i < m; i++) {
                int u = (v + pfs[i]) % pfs[0];
                if (dist[u] > d + pfs[i]) {
                    dist[u] = d + pfs[i];
                    pq.emplace(d + pfs[i], u);
                }
            }
        }
        return dist[K % pfs[0]] <= K;
    };
    if (!check(k) || !check(n - k)) {
        cout << "O nei!";
        exit(0);
    }

    if (k > 1e5) {
        cout << "Hellingur!";
        exit(0);
    }

    int K = min(k, n - k);
    vector<int> dp(K + 1, 1e9), prev(K + 1);
    dp[0] = 0;
    for (int p = 1; p <= K; p++)
        for (int i = 0; i < pfs.size() && pfs[i] <= p; i++)
            if (dp[p] > dp[p - pfs[i]] + 1) {
                dp[p] = dp[p - pfs[i]] + 1;
                prev[p] = i;
            }

    vector<int> count(m, 0);
    for (int p = K; p;) {
        int i = prev[p];
        count[i]++;
        p -= pfs[i];
    }

    int nodes = 0;
    vector<unordered_map<long long, int>> compress(m);
    DisjointSets dsu(m * K + 1);
    vector<long long> rep(m * K + 1);
    vector<int> used(m, 0);
    auto get = [&](int i, auto v) {
        auto it = compress[i].find(v);
        return it == compress[i].end() ? v : rep[dsu.find(it->second)];
    };

    vector<long long> slots;
    for (int i = m - 1; ~i; i--)
        if (count[i]) {
            auto q1 = n / pfs[i];
            for (; count[i]; count[i]--) {
                auto start = get(i, 0);
                for (; start < q1; start = get(i, start + 1)) {
                    for (int j = 0; j < m; j++) {
                        int c = count[j];
                        if (!c) continue;

                        auto q2 = n / pfs[j];
                        if (i != j) {
                            auto r = start % q2, d = q1 % q2;
                            for (int _ = 0; _ < pfs[i]; _++) {
                                c += get(j, r) == r;
                                r = (r + d) % q2;
                            }
                        }
                        if (q2 - used[j] < c) goto next;
                    }
                    break;
                    next:;
                }
                for (int j = 0; j < pfs[i]; j++) slots.emplace_back(start + q1 * j);
                
                for (int j = 0; j < m; j++) {
                    auto use = [&](auto v) {
                        if (compress[j].count(v)) return;

                        int node = nodes++;
                        compress[j][v] = node;
                        rep[node] = v + 1;
                        for (auto u: {v - 1, v + 1})
                            if (compress[j].count(u)) {
                                auto [big, small] = dsu.unite(node, compress[j][u]);
                                rep[big] = max(rep[big], rep[small]);
                                node = big;
                            }
                        used[j]++;
                    };

                    if (i == j) use(start);
                    else {
                        auto q2 = n / pfs[j], r = start % q2, d = q1 % q2;
                        for (int _ = 0; _ < pfs[i]; _++) {
                            use(r);
                            r = (r + d) % q2;
                        }
                    }
                }
            }
        }

    if (K != k) {
        vector<bool> valid(n, true);
        for (int v : slots) valid[v] = false;
        for (int v = 0; v < n; v++)
            if (valid[v]) cout << v + 1 << " ";
    } else
        for (auto v : slots) cout << v + 1 << " ";
}
