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

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        int components = n;
        DisjointSets dsu1(n);
        vector<pair<array<int, 3>, int>> edges(m);
        for (int i = 0; i < m; i++) {
            auto &[w, u, v] = edges[i].first;
            cin >> u >> v >> w;
            u--;
            v--;

            components -= dsu1.unite(u, v);
            edges[i].second = i + 1;
        }

        if (components != 1) {
            cout << "-1\n";
            continue;
        }

        DisjointSets dsu2(n);
        sort(edges.begin(), edges.end());
        vector<int> indices;
        for (int l = 0, r = 0; l < m;) {
            for (; r < m && edges[l].first[0] == edges[r].first[0]; r++);

            vector<pair<array<int, 3>, int>> candidates;
            for (; l < r; l++) {
                auto [w, u, v] = edges[l].first;
                if (dsu2.find(u) != dsu2.find(v)) candidates.emplace_back(edges[l]);
            }

            for (auto [e, i] : candidates) {
                auto [w, u, v] = e;
                if (dsu2.find(u) == dsu2.find(v)) indices.emplace_back(i);
                dsu2.unite(u, v);
            }
        }

        cout << indices.size() << " ";
        for (int i : indices) cout << i << " ";
        cout << "\n";
    }
}