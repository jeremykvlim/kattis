#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    int t;
    vector<int> sets, seen;

    int find(int v) {
        if (seen[v] != t) {
            seen[v] = t;
            sets[v] = -1;
            return v;
        }

        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    void reset() {
        t++;
    }

    DisjointSets(int n) : t(1), sets(n, -1), seen(n, 0) {}
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
