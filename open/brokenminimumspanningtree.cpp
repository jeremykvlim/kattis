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

    void reset() {
        iota(sets.begin(), sets.end(), 0);
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

    vector<array<int, 4>> edges(m), tree_edges(n - 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        edges[i] = {w, i, u, v};
        if (i < n - 1) tree_edges[i] = {w, i, u, v};
    }
    sort(edges.begin(), edges.end());

    DisjointSets dsu(n + 1);
    vector<bool> removed(m, true);
    vector<pair<int, int>> mst_edges;
    vector<array<int, 4>> add, remove;
    for (auto [w, i, u, v] : edges)
        if (dsu.unite(u, v)) {
            if (i < n - 1) {
                removed[i] = false;
                mst_edges.emplace_back(u, v);
            } else add.push_back({w, i, u, v});
        }

    for (int i = 0; i < n - 1; i++)
        if (removed[i]) remove.emplace_back(tree_edges[i]);

    vector<pair<int, int>> swaps;
    for (int i = remove.size() - 1; ~i; i--) {
        dsu.reset();
        for (auto [u, v] : mst_edges) dsu.unite(u, v);

        for (int j = 0; j < i; j++) {
            auto [_, __, u, v] = remove[j];
            dsu.unite(u, v);
        }

        auto [_, a, __, ___] = remove[i];
        for (int j = 0; j < add.size(); j++) {
            auto [w, b, u, v] = add[j];
            if (dsu.unite(u, v)) {
                mst_edges.emplace_back(u, v);
                swaps.emplace_back(a, b);
                add.erase(add.begin() + j);
                break;
            }
        }
    }

    cout << swaps.size() << "\n";
    for (auto [a, b] : swaps) cout << a + 1 << " " << b + 1 << "\n";
}
