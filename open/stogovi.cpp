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

vector<int> tarjan_lca(int n, int q, const vector<vector<int>> &adj_list, const vector<vector<pair<int, int>>> &queries) {
    DisjointSets dsu(n);
    vector<bool> visited(n, false);
    vector<int> lca(q, -1), ancestor(n, -1);

    auto dfs = [&](auto &&self, int v = 0) -> void {
        visited[v] = true;
        ancestor[v] = v;

        for (int u : adj_list[v])
            if (!visited[u]) {
                self(self, u);
                dsu.unite(v, u);
                ancestor[dsu.find(v)] = v;
            }

        for (auto &[u, i] : queries[v])
            if (visited[u]) lca[i] = ancestor[dsu.find(u)];
    };
    dfs(dfs);
    return lca;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> id(n + 1, 0), parent(n + 1, 0), op(n + 1, -1), depth(n + 1, 0);
    vector<vector<int>> adj_list(n + 1);
    vector<vector<pair<int, int>>> queries(n + 1);
    for (int i = 1; i <= n; i++) {
        char c;
        int v;
        cin >> c >> v;

        if (c == 'a') {
            id[i] = i;
            depth[i] = depth[parent[i] = id[v]] + 1;
            adj_list[id[v]].emplace_back(i);
        } else if (c == 'b') {
            id[i] = parent[id[v]];
            op[i] = id[v];
        } else {
            int w;
            cin >> w;

            id[i] = id[v];
            queries[id[v]].emplace_back(id[w], i);
            queries[id[w]].emplace_back(id[v], i);
        }
    }

    auto lca = tarjan_lca(n + 1, n + 1, adj_list, queries);
    for (int i = 1; i <= n; i++) {
        if (~lca[i]) op[i] = depth[lca[i]];
        if (~op[i]) cout << op[i] << "\n";
    }
}
