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

    vector<array<int, 3>> edges(m);
    for (auto &[w, u, v] : edges) cin >> u >> v >> w;
    sort(edges.begin(), edges.end());

    auto total = 0LL;
    vector<bool> mst_edge(m, false);
    DisjointSets dsu1(n + 1);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        auto [w, u, v] = edges[i];
        if (dsu1.find(u) != dsu1.find(v)) {
            dsu1.unite(u, v);
            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
            total += w;
            mst_edge[i] = true;
        }
    }

    int count = 0;
    vector<pair<int, int>> prev(n + 1, {-1, -1});
    vector<int> depth(n + 1, 0), in(n + 1), out(n + 1);
    auto dfs = [&](auto &&self, int v = 1) -> void {
        in[v] = count++;
        for (auto [u, w] : adj_list[v])
            if (u != prev[v].first) {
                prev[u] = {v, w};
                depth[u] = depth[v] + 1;
                self(self, u);
            }
        out[v] = count;
    };
    dfs(dfs);

    auto ancestor = [&](int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    };

    DisjointSets dsu2(n + 1);
    vector<int> rep(n + 1, 0);
    iota(rep.begin(), rep.end(), 0);
    auto weight = total;
    for (int i = 0; i < m; i++)
        if (!mst_edge[i]) {
            auto [w, u, v] = edges[i];
            auto walk = [&](int &t) {
                auto [p, d] = prev[t];
                weight = max(weight, total - d + w);
                int t_set = dsu2.find(t), p_set = dsu2.find(p);
                dsu2.unite(t_set, p_set);
                rep[dsu2.find(t_set)] = depth[rep[t_set]] < depth[rep[p_set]] ? rep[t_set] : rep[p_set];
                t = rep[dsu2.find(p_set)];
            };

            int u_rep = rep[dsu2.find(u)], v_rep = rep[dsu2.find(v)];
            while (!ancestor(u_rep, v_rep)) walk(u_rep);
            while (dsu2.find(u_rep) != dsu2.find(v_rep)) walk(v_rep);
        }
    cout << weight;
}
