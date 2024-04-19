#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> endpoints(m);
    vector<vector<int>> adj_list(n + 1);
    vector<long long> count(n + 1, 0);
    for (auto &[u, v] : endpoints) {
        cin >> u >> v;
        
        count[u]++;
        count[v]++;
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    auto subtrees = 0LL;
    for (int u = 1; u <= n; u++) {
        for (int v: adj_list[u]) subtrees += (count[u] - 1) * (count[u] - 2) * (count[u] - 3) / 6 * (count[v] - 1);
        adj_list[u].clear();
    }

    for (auto [u, v] : endpoints)
        (count[u] > count[v] || count[u] == count[v] && u > v) ? adj_list[v].emplace_back(u) : adj_list[u].emplace_back(v);

    vector<int> parent(n + 1, 0);
    for (int u = 1; u <= n; u++) {
        for (int v : adj_list[u]) parent[v] = u;
        for (int v : adj_list[u])
            for (int w : adj_list[v])
                if (parent[w] == u) subtrees -= (count[u] - 2) * (count[u] - 3) + (count[v] - 2) * (count[v] - 3) + (count[w] - 2) * (count[w] - 3);
    }

    cout << subtrees;
}
