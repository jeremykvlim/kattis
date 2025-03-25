#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
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

    DisjointSets dsu(n + 1);
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
