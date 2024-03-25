#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> factors(n * n + 1), primes{2};
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; primes.size() < n; p += 2)
        if (factors[p] == p) {
            primes.emplace_back(p);
            for (auto i = (long long) p * p; i <= n * n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

void dfs(vector<vector<int>> &adj_list, vector<int> &prev, int v = 0) {
    for (int u : adj_list[v]) {
        if (u == prev[v]) continue;

        prev[u] = v;
        dfs(adj_list, prev, u);
    }
}

pair<int, long long> depth(vector<int> &prev, vector<long long> &labels, int v) {
    if (labels[v]) return {0, labels[v]};

    auto [d, x] = depth(prev, labels, prev[v]);
    return {d + 1, x};
}

void update(vector<int> &prev, vector<long long> &labels, int v, int p) {
    if (!labels[prev[v]]) update(prev, labels, prev[v], p);

    labels[v] = labels[prev[v]] * p;
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

    vector<int> primes = sieve(n), prev(n, -1);
    dfs(adj_list, prev);

    vector<long long> labels(n, 0);
    labels[0] = 1;
    for (int p : primes) {
        pair<int, long long> deepest{1, 0};
        int v = -1;
        for (int u = 0; u < n; u++) {
            auto d = depth(prev, labels, u);
            if (deepest < d) {
                deepest = d;
                v = u;
            }
        }

        if (v == -1) break;
        update(prev, labels, v, p);
    }

    for (auto x : labels) cout << x << " ";
}
