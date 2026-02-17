#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, e;
    cin >> V >> e;

    vector<vector<int>> adj_list(V + 1);
    vector<int> degree(V + 1, 0);
    while (e--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        degree[v]++;
    }

    set<int> q;
    for (int i = 1; i <= V; i++)
        if (!degree[i]) q.emplace(i);

    auto c = 0LL;
    for (int i = 1; i <= V; i++) {
        auto it = i & 1 ? q.begin() : prev(q.end());
        int v = *it;
        q.erase(it);

        for (int u : adj_list[v])
            if (!--degree[u]) q.emplace(u);

        c += (long long) i * v;
    }
    cout << c;
}
