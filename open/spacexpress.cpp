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

    int t;
    cin >> t;

    while (t--) {
        int ns;
        cin >> ns;

        unordered_set<char> all;
        vector<pair<char, char>> edges_s;
        while (ns--) {
            char u, v;
            cin >> u >> v;

            edges_s.emplace_back(u, v);
            all.emplace(u);
            all.emplace(v);
        }

        int nc;
        cin >> nc;

        vector<pair<char, char>> edges_c;
        while (nc--) {
            char u, v;
            cin >> u >> v;

            edges_c.emplace_back(u, v);
            all.emplace(u);
            all.emplace(v);
        }

        int count = 0;
        unordered_map<char, int> indices;
        for (char c : all) indices[c] = count++;

        int n = all.size();
        DisjointSets dsu1(n), dsu2(n);
        for (auto [u, v] : edges_s) dsu1.unite(indices[u], indices[v]);
        for (auto [u, v] : edges_c) dsu2.unite(indices[u], indices[v]);

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if ((dsu1.find(i) == dsu1.find(j)) ^ (dsu2.find(i) == dsu2.find(j))) {
                    cout << "NO\n\n";
                    goto next;
                }

        cout << "YES\n\n";
        next:;
    }
}
