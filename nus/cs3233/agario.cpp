#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<array<int, 3>> edges(m);
    for (auto &[w, u, v] : edges) {
        cin >> u >> v;

        w = max(a[u], a[v]);
        if (a[u] < a[v]) swap(u, v);
    }
    sort(edges.begin(), edges.end());

    DisjointSet dsu(n + 1);
    vector<vector<int>> adj_list(n + 1);
    vector<bool> visited(n + 1, false);
    for (auto [w, u, v] : edges)
        if (dsu.unite(u, v)) {
            if (visited[v]) v = dsu.sets[v];
            adj_list[u].emplace_back(v);
            visited[v] = true;
        }

    vector<long long> size(a.begin(), a.end());

    auto dfs1 = [&](auto &&self, int v) -> void {
        for (int u : adj_list[v]) {
            self(self, u);
            size[v] += size[u];
        }
    };

    auto dfs2 = [&](auto &&self, int v, int prev = -1) -> void {
        if (prev != -1 && size[v] >= a[prev]) size[v] = size[prev];
        for (int u : adj_list[v]) self(self, u, v);
    };

    for (int i = 1; i <= n; i++)
        if (dsu.sets[i] == i) {
            dfs1(dfs1, i);
            dfs2(dfs2, i);
            break;
        }

    for (int i = 1; i <= n; i++) cout << size[i] << " ";
}
