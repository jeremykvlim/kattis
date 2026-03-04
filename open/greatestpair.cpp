#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
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
    return spf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> label(1e5 + 1), heavy(1e5 + 1), spf = sieve(5e5), indices(5e5 + 1);
    vector<long long> depth(1e5 + 1), dist(5e5 + 1, -1);
    vector<vector<pair<int, int>>> adj_list(1e5 + 1);
    vector<vector<int>> active(1e5 + 1);
    vector<vector<long long>> active_dists(1e5 + 1);

    int n;
    while (cin >> n && n) {
        vector<int> labels;
        for (int i = 1; i <= n; i++) {
            cin >> label[i];

            labels.emplace_back(label[i]);
            heavy[i] = depth[i] = 0;
            adj_list[i].clear();
            active[i].clear();
            active_dists[i].clear();
        }
        sort(labels.begin(), labels.end());
        labels.erase(unique(labels.begin(), labels.end()), labels.end());

        for (int _ = 0; _ < n - 1; _++) {
            int u, v, w;
            cin >> u >> v >> w;

            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
        }

        vector<vector<int>> divs(labels.size());
        for (int i = 0; i < labels.size(); i++) {
            indices[labels[i]] = i;
            vector<pair<int, int>> pfs;
            for (int temp = labels[i]; temp > 1;) {
                int pf = spf[temp], pow = 0;
                for (; !(temp % pf); temp /= pf, pow++);
                pfs.emplace_back(pf, pow);
            }

            divs[i].emplace_back(1);
            for (auto [pf, pow] : pfs) {
                int pp = pf, d = divs[i].size();
                while (pow--) {
                    for (int j = 0; j < d; j++) divs[i].emplace_back(divs[i][j] * pp);
                    pp *= pf;
                }
            }
        }

        auto hld = [&](auto &&self, int v = 1, int prev = -1) -> int {
            int subtree_size = 1, largest = 0;
            for (auto [u, w] : adj_list[v])
                if (u != prev) {
                    depth[u] = depth[v] + w;
                    int size = self(self, u, v);
                    subtree_size += size;
                    if (largest < size) {
                        largest = size;
                        heavy[v] = u;
                    }
                }
            return subtree_size;
        };
        hld(hld);

        auto g = 0LL;
        auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
            for (auto [u, w] : adj_list[v])
                if (u != prev && u != heavy[v]) {
                    self(self, u, v);

                    for (int d : active[u]) {
                        active_dists[u].emplace_back(dist[d]);
                        dist[d] = -1;
                    }
                }

            if (heavy[v]) {
                self(self, heavy[v], v);
                swap(active[v], active[heavy[v]]);
            }

            for (int d : divs[indices[label[v]]]) {
                if (~dist[d]) g = max(g, (dist[d] - depth[v]) * d);
                else active[v].emplace_back(d);
                dist[d] = max(dist[d], depth[v]);
            }

            for (auto [u, w] : adj_list[v])
                if (u != prev && u != heavy[v])
                    for (int i = 0; i < active[u].size(); i++) {
                        int d = active[u][i];
                        if (~dist[d]) g = max(g, (dist[d] + active_dists[u][i] - 2 * depth[v]) * d);
                        else active[v].emplace_back(d);
                        dist[d] = max(dist[d], active_dists[u][i]);
                    }
        };
        dfs(dfs);

        cout << g << "\n";
        for (int d : active[1]) dist[d] = -1;
    }
}
