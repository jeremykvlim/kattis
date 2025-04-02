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

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> tube(n, -1);
    vector<vector<int>> adj_list(n);
    DisjointSets dsu(n);
    while (m--) {
        int a, b;
        cin >> a >> b;

        if (tube[a] != -1 || !dsu.unite(a, b)) {
            cout << "NO";
            exit(0);
        }
        tube[a] = b;
        adj_list[b].emplace_back(a);
    }

    vector<unordered_set<int>> no_tube(n);
    while (k--) {
        int c, d;
        cin >> c >> d;

        no_tube[d].emplace(c);
    }

    unordered_set<int> unconnected;
    for (int i = 0; i < n; i++)
        if (tube[i] == -1) unconnected.emplace(i);

    int root = -1;
    queue<int> q;
    for (int i = 0; i < n; i++)
        if (unconnected.count(i) && tube[i] == -1) {
            root = i;

            unconnected.erase(i);
            q.emplace(i);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                unordered_set<int> temp;
                for (int u : adj_list[v]) q.emplace(u);

                for (int u : unconnected) 
                    if (no_tube[v].count(u)) temp.emplace(u);
                    else q.emplace(u);
                unconnected = temp;
            }
        }

    unconnected.clear();
    for (int i = 0; i < n; i++)
        if (tube[i] == -1) unconnected.insert(i);

    vector<pair<int, int>> tree;
    unconnected.erase(root);
    q.emplace(root);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        unordered_set<int> temp;
        for (int u : adj_list[v]) {
            tree.emplace_back(u, v);
            q.emplace(u);
        }

        for (int u : unconnected)
            if (no_tube[v].count(u)) temp.emplace(u);
            else {
                tree.emplace_back(u, v);
                q.emplace(u);
            }
        unconnected = temp;
    }

    if (tree.size() != n - 1) cout << "NO";
    else
        for (auto [u, v] : tree) cout << u << " " << v << "\n";
}
