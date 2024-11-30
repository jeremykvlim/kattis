#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        DisjointSet dsu1(n);
        vector<pair<array<int, 3>, int>> edges(m);
        for (int i = 0; i < m; i++) {
            auto &[w, u, v] = edges[i].first;
            cin >> u >> v >> w;
            u--;
            v--;

            dsu1.unite(u, v);
            edges[i].second = i + 1;
        }

        if (count_if(dsu1.sets.begin(), dsu1.sets.end(), [i = 0](int s) mutable {return s == i++;}) != 1) {
            cout << "-1\n";
            continue;
        }

        DisjointSet dsu2(n);
        sort(edges.begin(), edges.end());
        vector<int> indices;
        for (int l = 0, r = 0; l < m;) {
            while (r < m && edges[l].first[0] == edges[r].first[0]) r++;

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
