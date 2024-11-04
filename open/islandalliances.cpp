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

    int n, m, q;
    cin >> n >> m >> q;

    vector<unordered_set<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace(v);
        adj_list[v].emplace(u);
    }

    DisjointSet dsu(n + 1);
    while (q--) {
        int a, b;
        cin >> a >> b;

        int a_set = dsu.find(a), b_set = dsu.find(b);

        if (adj_list[a_set].count(b_set) || adj_list[b_set].count(a_set)) {
            cout << "REFUSE\n";
            continue;
        }

        cout << "APPROVE\n";
        if (adj_list[a_set].size() < adj_list[b_set].size()) swap(a_set, b_set);
        dsu.unite(a_set, b_set);
        for (int v : adj_list[b_set]) adj_list[a_set].emplace(dsu.find(v));
    }
}
