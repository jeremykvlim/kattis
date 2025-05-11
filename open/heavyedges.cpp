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

vector<int> tarjan(int n, vector<vector<pair<int, int>>> &adj_list, vector<int> &sources) {
    static vector<int> order(n + 1, 0), low(n + 1, 0);
    static int count = 0;
    vector<bool> visited(n + 1, false);
    vector<int> bridges;

    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        visited[v] = true;

        for (auto [u, i] : adj_list[v])
            if (i != prev) {
                if (!visited[u]) {
                    self(self, u, i);
                    low[v] = min(low[v], low[u]);
                    if (low[u] > order[v]) bridges.emplace_back(i);
                } else low[v] = min(low[v], order[u]);
            }
    };

    for (int v : sources)
        if (!visited[v]) dfs(dfs, v);

    return bridges;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 4>> edges(m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        edges[i] = {w, u, v, i};
    }
    sort(edges.begin(), edges.end());

    DisjointSets dsu(n + 1);
    vector<bool> heavy(m, false);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0, j = 1; i < m; i = j++) {
        while (j < m && edges[i][0] == edges[j][0]) j++;

        vector<int> sources;
        vector<array<int, 3>> temp;
        for (int k = i; k < j; k++) {
            auto [w, u, v, e] = edges[k];

            int u_set = dsu.find(u), v_set = dsu.find(v);
            if (u_set == v_set) heavy[e] = true;
            else {
                sources.emplace_back(u_set);
                sources.emplace_back(v_set);
                temp.push_back({u_set, v_set, e});
            }
        }

        if (!sources.empty()) {
            sort(sources.begin(), sources.end());
            sources.erase(unique(sources.begin(), sources.end()), sources.end());

            for (int k = 0; k < temp.size(); k++) {
                auto [u, v, e] = temp[k];

                adj_list[u].emplace_back(v, k);
                adj_list[v].emplace_back(u, k);
            }

            auto bridges = tarjan(n, adj_list, sources);
            sort(bridges.begin(), bridges.end());

            for (int k = 0; k < temp.size(); k++)
                if (!binary_search(bridges.begin(), bridges.end(), k)) heavy[temp[k][2]] = true;

            for (int v : sources) adj_list[v].clear();
        }

        for (int k = i; k < j; k++) dsu.unite(edges[k][1], edges[k][2]);
    }

    cout << count_if(heavy.begin(), heavy.end(), [&](bool h) { return h; });
}
