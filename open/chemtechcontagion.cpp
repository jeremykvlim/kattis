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

    vector<int> parent(n + 1), depth(n + 1, 0), subtree_size(n + 1), heavy(n + 1, -1);
    vector<long long> degree_sum(n + 1, 0);
    auto dfs1 = [&](auto &&self, int v = 1, int prev = -1) -> void {
        subtree_size[v] = 1;
        for (int u : adj_list[v])
            if (u != prev) {
                parent[u] = v;
                depth[u] = depth[v] + 1;
                degree_sum[u] = degree_sum[v] + degree[u];
                self(self, u, v);
                subtree_size[v] += subtree_size[u];
                if (subtree_size[u] > subtree_size[heavy[v]]) heavy[v] = u;
            }
    };
    dfs1(dfs1);

    int count = 0;
    vector<int> order(n + 1), head(n + 1);
    auto dfs2 = [&](auto &&self, int v = 1, int h = 1) -> void {
        head[v] = h;
        order[v] = ++count;
        if (heavy[v] != -1) self(self, heavy[v], h);

        for (int u : adj_list[v])
            if (u != parent[v] && u != heavy[v]) self(self, u, u);
    };
    dfs2(dfs2);

    auto lca = [&](int u, int v) {
        while (head[u] != head[v])
            if (depth[head[u]] > depth[head[v]]) u = parent[head[u]];
            else v = parent[head[v]];

        return depth[u] < depth[v] ? u : v;
    };

    vector<long long> pipes(q);
    vector<pair<int, int>> queries(q);
    vector<vector<int>> indices(n + 1);
    for (int i = 0; i < q; i++) {
        auto &[a, b] = queries[i];
        cin >> a >> b;

        int anc = lca(a, b);
        pipes[i] = degree_sum[a] + degree_sum[b] - 2 * degree_sum[anc] + degree[anc];
        indices[a].emplace_back(i);
        indices[b].emplace_back(i);
    }

    vector<vector<int>> sweep(n + 1);
    for (auto [u, v] : edges) {
        if (depth[u] >= depth[v]) swap(u, v);
        sweep[v].emplace_back(u);
    }

    FenwickTree<long long> fw(n + 1);
    auto dfs3 = [&](auto &&self, int v = 1, int prev = -1) -> void {
        for (int u : sweep[v]) fw.update(order[u], 1);

        for (int i : indices[v]) {
            auto [a, b] = queries[i];

            auto p = 0LL;
            while (head[a] != head[b])
                if (depth[head[a]] > depth[head[b]]) {
                    p += fw.pref_sum(order[a]) - fw.pref_sum(order[head[a]] - 1);
                    a = parent[head[a]];
                } else {
                    p += fw.pref_sum(order[b]) - fw.pref_sum(order[head[b]] - 1);
                    b = parent[head[b]];
                }

            auto [l, r] = minmax(order[a], order[b]);
            pipes[i] -= 2 * (p + fw.pref_sum(r) - fw.pref_sum(l - 1));
        }

        for (int u : adj_list[v])
            if (u != prev) self(self, u, v);

        for (int u : sweep[v]) fw.update(order[u], -1);
    };
    dfs3(dfs3);

    for (auto p : pipes) cout << p << "\n";
}
