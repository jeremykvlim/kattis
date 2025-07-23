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

    if (m & 1) {
        cout << "impossible";
        exit(0);
    }

    DisjointSets dsu(n);
    vector<vector<pair<int, int>>> adj_list_full(n), adj_list_tree(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        adj_list_full[a].emplace_back(b, i);
        adj_list_full[b].emplace_back(a, i);
        if (dsu.unite(a, b)) {
            adj_list_tree[a].emplace_back(b, i);
            adj_list_tree[b].emplace_back(a, i);
        }
    }

    vector<pair<int, int>> pairs;
    vector<bool> paired(m, false);
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (auto [u, i] : adj_list_tree[v])
            if (u != prev) self(self, u, v);

        int j = -1, k = -1;
        for (auto [u, i] : adj_list_full[v]) {
            if (u == prev) {
                k = i;
                continue;
            }

            if (!paired[i]) {
                if (j == -1) j = i;
                else {
                    pairs.emplace_back(j, i);
                    j = -1;
                }
                paired[i] = true;
            }
        }

        if (~j) {
            pairs.emplace_back(j, k);
            if (k != -1) paired[k] = true;
        }
    };
    dfs(dfs);

    for (auto [i, j] : pairs) cout << i << " " << j << "\n";
}
