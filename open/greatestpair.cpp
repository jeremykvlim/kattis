#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }

    vector<int> exponent(n + 1, 1), base(n + 1, 1);
    vector<vector<int>> divisors(n + 1);
    divisors[1] = {1};
    for (int i = 2; i <= n; i++) {
        int p = spf[i], m = i / p;

        if (spf[m] == p) {
            exponent[i] = exponent[m] + 1;
            base[i] = base[m];
        } else base[i] = m;

        vector<int> prime_powers(exponent[i] + 1, 1);
        for (int k = 1; k <= exponent[i]; k++) prime_powers[k] = prime_powers[k - 1] * p;

        for (int d : divisors[base[i]])
            for (int pp : prime_powers) divisors[i].emplace_back(d * pp);
    }

    return divisors;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto divisors = sieve(5e5);

    vector<int> label(1e5 + 1), subtree_size(1e5 + 1), heavy(1e5 + 1);
    vector<long long> depth(1e5 + 1), dist(5e5 + 1, -1);
    vector<vector<pair<int, int>>> adj_list(1e5 + 1);
    vector<vector<int>> bag(1e5 + 1);
    vector<vector<long long>> bag_dist(1e5 + 1);

    int n;
    while (cin >> n && n) {
        for (int i = 1; i <= n; i++) {
            cin >> label[i];

            heavy[i] = depth[i] = 0;
            adj_list[i].clear();
            bag[i].clear();
            bag_dist[i].clear();
        }

        for (int _ = 0; _ < n - 1; _++) {
            int u, v, w;
            cin >> u >> v >> w;

            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
        }

        auto dfs1 = [&](auto &&self, int v = 1, int prev = -1) -> void {
            subtree_size[v] = 1;
            for (auto [u, w] : adj_list[v])
                if (u != prev) {
                    depth[u] = depth[v] + w;
                    self(self, u, v);
                    subtree_size[v] += subtree_size[u];
                    if (subtree_size[u] > subtree_size[heavy[v]]) heavy[v] = u;
                }
        };
        dfs1(dfs1);

        auto g = 0LL;
        auto dfs2 = [&](auto &&self, int v = 1, int prev = -1) -> void {
            for (auto [u, w] : adj_list[v])
                if (u != prev && u != heavy[v]) {
                    self(self, u, v);

                    for (int d : bag[u]) {
                        bag_dist[u].emplace_back(dist[d]);
                        dist[d] = -1;
                    }
                }

            if (heavy[v]) {
                self(self, heavy[v], v);
                swap(bag[v], bag[heavy[v]]);
            }

            for (int d : divisors[label[v]])
                if (dist[d] != -1) g = max(g, (dist[d] - depth[v]) * d);

            for (int d : divisors[label[v]]) {
                if (dist[d] == -1) bag[v].emplace_back(d);
                dist[d] = max(dist[d], depth[v]);
            }

            for (auto [u, w] : adj_list[v])
                if (u != prev && u != heavy[v]) {
                    for (int i = 0; i < bag[u].size(); i++) {
                        int d = bag[u][i];
                        if (dist[d] != -1) g = max(g, (dist[d] + bag_dist[u][i] - 2 * depth[v]) * d);
                    }

                    for (int i = 0; i < bag[u].size(); i++) {
                        int d = bag[u][i];
                        if (dist[d] == -1) bag[v].emplace_back(d);
                        dist[d] = max(dist[d], bag_dist[u][i]);
                    }
                }
        };
        dfs2(dfs2);

        cout << g << "\n";
        for (int d : bag[1]) dist[d] = -1;
    }
}
