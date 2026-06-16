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

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
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

    vector<unordered_set<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace(v);
        adj_list[v].emplace(u);
    }

    DisjointSets dsu(n + 1);
    while (q--) {
        int a, b;
        cin >> a >> b;

        int a_set = dsu.find(a), b_set = dsu.find(b);

        if (adj_list[a_set].count(b_set) || adj_list[b_set].count(a_set)) {
            cout << "REFUSE\n";
            continue;
        }

        cout << "APPROVE\n";
        auto [big, small] = dsu.unite(a_set, b_set);
        for (int v : adj_list[small]) {
            int v_set = dsu.find(v);
            if (v_set != big) adj_list[big].emplace(v_set);
        }
        adj_list[small].clear();
    }
}