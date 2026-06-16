#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
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

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    for (auto &[l, u, v] : edges) cin >> u >> v >> l;
    sort(edges.begin(), edges.end());

    DisjointSets dsu(n);
    for (int i = 0; i < m;) {
        int j = i;
        for (; j < m && edges[i][0] == edges[j][0]; j++)
            if (dsu.find(edges[j][1]) == dsu.find(edges[j][2])) {
                cout << "not ultrametric";
                exit(0);
            }

        for (; i < j; i++) dsu.unite(edges[i][1], edges[i][2]);
    }
    cout << "possibly ultrametric";
}
