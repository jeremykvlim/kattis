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

tuple<int, vector<bool>, vector<vector<pair<int, int>>>> kruskal(int n, vector<array<int, 4>> &edges) {
    DisjointSets dsu(n);
    sort(edges.begin(), edges.end());

    int len = 0, count = 0;
    vector<bool> in_mst(edges.size(), false);
    vector<vector<pair<int, int>>> adj_list(n);
    for (auto [w, u, v, i] : edges)
        if (dsu.unite(u, v)) {
            len += w;
            in_mst[i] = true;
            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
            if (++count == n - 1) break;
        }

    return {len, in_mst, adj_list};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, b;
    cin >> V >> b;

    vector<array<int, 4>> edges(b);
    for (int i = 0; i < b; i++) {
        int u, v, c;
        cin >> u >> v >> c;

        edges[i] = {c, u, v, i};
    }
    auto [len, in_mst, adj_list] = kruskal(V, edges);

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(V, 0), inlabel(V), ascendant(V, 0), head(V + 1);
    vector<pair<int, int>> prev(V, {0, 0});
    auto dfs = [&](auto &&self, int v = 0) -> void {
        tour.emplace_back(v, prev[v].first);
        inlabel[v] = tour.size();

        for (auto [u, w] : adj_list[v])
            if (u != prev[v].first) {
                prev[u] = {v, w};
                depth[u] = depth[v] + 1;
                self(self, u);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);
    for (auto [v, p] : tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    DisjointSets dsu(V);
    vector<int> replacement(V, 1e9);
    for (auto [c, u, v, i] : edges) {
        if (in_mst[i]) continue;
        int a = lca(u, v);
        auto climb = [&](int t) {
            for (;;) {
                t = dsu.find(t);
                if (depth[t] <= depth[a]) break;
                replacement[t] = c;
                dsu.unite(prev[t].first, t);
            }
        };
        climb(u);
        climb(v);
    }

    int cost = len;
    for (int v = 1; v < V; v++)
        if (replacement[v] != 1e9) cost = max(cost, len - prev[v].second + replacement[v]);
    cout << cost;
}
