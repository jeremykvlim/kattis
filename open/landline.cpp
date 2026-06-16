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

    int n, m, p;
    cin >> n >> m >> p;

    vector<bool> insecure(n + 1, false);
    while (p--) {
        int a;
        cin >> a;

        insecure[a] = true;
    }

    vector<array<int, 3>> edges(m);
    for (auto &[li, xi, yi] : edges) cin >> xi >> yi >> li;
    sort(edges.begin(), edges.end());

    DisjointSets dsu(n + 1);
    vector<bool> visited(n + 1, false);
    int cost = 0, mst_edges = 0;
    for (auto [l, x, y] : edges) {
        if ((insecure[x] && visited[x]) || (insecure[y] && visited[y]) || (n > 2 && insecure[x] && insecure[y])) continue;
        visited[x] = visited[y] = true;
        if (dsu.find(x) != dsu.find(y)) {
            dsu.unite(x, y);
            cost += l;
            mst_edges++;
        }
    }

    if (mst_edges != n - 1) cout << "impossible";
    else cout << cost;
}
