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
    for (auto &p : endpoints) {
        cin >> p.first >> p.second;
        count[p.first]++;
        count[p.second]++;
        adj_list[p.first].emplace_back(p.second);
        adj_list[p.second].emplace_back(p.first);
    }

    long long subtrees = 0;
    for (int u = 1; u <= n; u++)
        for (auto &v : adj_list[u]) subtrees += (count[u] - 1) * (count[u] - 2) * (count[u] - 3) / 6 * (count[v] - 1);

    for (int i = 1; i <= n; i++) adj_list[i].clear();
    for (auto [u, v] : endpoints)
        if (count[u] > count[v] || count[u] == count[v] && u > v) adj_list[v].emplace_back(u);
        else adj_list[u].emplace_back(v);

    vector<int> parent(n + 1, 0);
    for (int u = 1; u <= n; u++) {
        for (auto &v : adj_list[u]) parent[v] = u;
        for (auto &v : adj_list[u])
            for (auto &w : adj_list[v])
                if (parent[w] == u) subtrees -= (count[u] - 2) * (count[u] - 3) + (count[v] - 2) * (count[v] - 3) + (count[w] - 2) * (count[w] - 3);
    }

    cout << subtrees;
}
