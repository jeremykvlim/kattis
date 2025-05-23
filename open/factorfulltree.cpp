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
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<int> primes = sieve(n * n), prev(n, -1);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
    };
    dfs(dfs);

    vector<long long> labels(n, 0);
    labels[0] = 1;
    for (int p : primes) {
        pair<int, long long> deepest{1, 0};
        int i = -1;
        for (int j = 0; j < n; j++) {
            auto depth = [&](auto &&self, int v) -> pair<int, long long> {
                if (labels[v]) return {0, labels[v]};

                auto [d, x] = self(self, prev[v]);
                return {d + 1, x};
            };

            auto d = depth(depth, j);
            if (deepest < d) {
                deepest = d;
                i = j;
            }
        }
        if (!~i) break;

        auto update = [&](auto &&self, int v) -> void {
            if (!labels[prev[v]]) self(self, prev[v]);

            labels[v] = labels[prev[v]] * p;
        };
        update(update, i);
    }

    for (auto x : labels) cout << x << " ";
}
