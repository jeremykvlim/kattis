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

int tarjan(int n, int m, vector<vector<pair<int, int>>> &adj_list) {
    vector<int> order(n, 0), low(n, 0);
    vector<bool> bridge(m, false);
    int count = 0;

    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        for (auto [u, i] : adj_list[v])
            if (i != prev) {
                if (!order[u]) {
                    self(self, u, i);
                    low[v] = min(low[v], low[u]);
                    if (low[u] > order[v]) bridge[i] = true;
                } else low[v] = min(low[v], order[u]);
            }
    };
    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return count_if(bridge.begin(), bridge.end(), [&](bool b) { return b; });
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    for (auto &[w, u, v] : edges) cin >> u >> v >> w;
    sort(edges.begin(), edges.end());

    DisjointSets dsu(n + 1);
    vector<int> indices(n + 1, -1);
    int heavy = 0;
    for (int l = 0, r = 1; l < m; l = r++) {
        for (; r < m && edges[l][0] == edges[r][0]; r++);

        int k = 0;
        vector<int> undo;
        vector<pair<int, int>> group_edges, unions;
        for (int i = l; i < r; i++) {
            auto [w, u, v] = edges[i];

            int u_set = dsu.find(u), v_set = dsu.find(v);
            if (u_set == v_set) {
                heavy++;
                continue;
            }

            if (!~indices[u_set]) {
                indices[u_set] = k++;
                undo.emplace_back(u_set);
            }
            if (!~indices[v_set]) {
                indices[v_set] = k++;
                undo.emplace_back(v_set);
            }
            group_edges.emplace_back(indices[u_set], indices[v_set]);
            unions.emplace_back(u_set, v_set);
        }

        int e = group_edges.size();
        if (e) {
            vector<vector<pair<int, int>>> adj_list(k);
            for (int i = 0; i < e; i++) {
                auto [u, v] = group_edges[i];
                adj_list[u].emplace_back(v, i);
                adj_list[v].emplace_back(u, i);
            }

            heavy += e - tarjan(k, e, adj_list);
        }

        for (auto [u, v] : unions) dsu.unite(u, v);
        for (int v : undo) indices[v] = -1;
    }
    cout << heavy;
}
