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
vector<T> factorize(T n) {
    vector<T> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs.emplace_back(m);
            return;
        }

        T d = pollard_brent(m);
        self(self, d);
        self(self, m / d);
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
