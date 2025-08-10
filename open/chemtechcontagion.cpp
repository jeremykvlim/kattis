#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> degree(n + 1, 0);
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) {
        cin >> u >> v;

        degree[u]++;
        degree[v]++;
    }

    vector<vector<int>> lift(__lg(n) + 1, vector<int>(n + 1, 1));
    vector<int> depth(n + 1, 0), in(n + 1), out(n + 1);
    vector<long long> degree_sum(n + 1, 0);
    int count = 0;
    auto dfs1 = [&](auto &&self, int v = 1, int prev = -1) -> void {
        in[v] = count++;
        if (~prev) {
            depth[v] = depth[prev] + 1;
            lift[0][v] = prev;
            for (int i = 1; i <= __lg(n); i++) lift[i][v] = lift[i - 1][lift[i - 1][v]];
        }

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                degree_sum[u] = degree_sum[v] + degree[u];
                self(self, u, v);
            }
        out[v] = count;
    };
    dfs1(dfs1);

    auto ancestor = [&](int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    };

    auto lca = [&](int u, int v) -> int {
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;

        if (depth[u] < depth[v]) swap(u, v);

        for (int i = __lg(n); ~i; i--)
            if (!ancestor(lift[i][u], v)) u = lift[i][u];

        return lift[0][u];
    };

    vector<vector<int>> sweep(n + 1);
    for (auto [u, v] : edges) {
        if (depth[u] >= depth[v]) swap(u, v);
        sweep[v].emplace_back(u);
    }

    vector<pair<int, int>> queries(q);
    vector<int> lcas(q);
    vector<long long> pipes(q);
    vector<vector<int>> indices(n + 1);
    for (int i = 0; i < q; i++) {
        auto &[a, b] = queries[i];
        cin >> a >> b;

        lcas[i] = lca(a, b);
        pipes[i] = degree_sum[a] + degree_sum[b] - 2 * degree_sum[lcas[i]] + degree[lcas[i]];
        indices[a].emplace_back(i);
        indices[b].emplace_back(i);
    }

    FenwickTree<long long> fw(n + 1);
    vector<int> visits(n + 1, 0);
    auto dfs2 = [&](auto &&self, int v = 1, int prev = -1) -> void {
        for (int u : sweep[v]) {
            fw.update(in[u] + 1, 1);
            fw.update(out[u] + 1, -1);
            visits[u]++;
        }

        for (int i : indices[v]) {
            auto [a, b] = queries[i];
            pipes[i] -= 2 * (fw.pref_sum(in[a] + 1) + fw.pref_sum(in[b] + 1) - 2 * fw.pref_sum(in[lcas[i]] + 1) + visits[lcas[i]]);
        }

        for (int u : adj_list[v])
            if (u != prev) self(self, u, v);

        for (int u : sweep[v]) {
            fw.update(in[u] + 1, -1);
            fw.update(out[u] + 1, 1);
            visits[u]--;
        }
    };
    dfs2(dfs2);

    for (auto p : pipes) cout << p << "\n";
}
