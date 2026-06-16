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

    int n, m, q;
    cin >> n >> m >> q;

    int t = 0;
    vector<array<int, 3>> edges(m);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;
        u--;
        v--;

        t = max(t, w);
    }

    vector<pair<int, int>> queries(q);
    for (auto &[a, b] : queries) {
        cin >> a >> b;
        a--;
        b--;
    }

    vector<vector<int>> indices1(t + 1);
    for (int i = 0; i < m; i++) indices1[edges[i][2]].emplace_back(i);

    vector<int> l(q, 0), r(q, t + 1), mid(q), k(q);
    bool change;
    do {
        change = false;
        vector<vector<int>> indices2(t + 1);
        for (int i = 0; i < q; i++) {
            mid[i] = l[i] + (r[i] - l[i]) / 2;

            indices2[mid[i]].emplace_back(i);
            if (l[i] + 1 < r[i]) change = true;
        }

        DisjointSets dsu(n);
        for (int w = 0; w <= t; w++) {
            for (int i : indices1[w]) dsu.unite(edges[i][0], edges[i][1]);
            for (int i : indices2[w]) {
                auto [a, b] = queries[i];
                if (dsu.find(a) == dsu.find(b)) {
                    r[i] = mid[i];
                    k[i] = dsu.size(a);
                } else l[i] = mid[i];
            }
        }
    } while (change);

    for (int i = 0; i < q; i++) cout << r[i] << " " << k[i] << "\n";
}