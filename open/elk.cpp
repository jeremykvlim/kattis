#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, a, b;
    cin >> n >> m >> a >> b;

    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<bool> visited(n, false), safe(n, true);
    safe[a] = safe[b] = false;
    auto dfs = [&](auto &&self, int v, int prev) -> bool {
        visited[v] = true;

        bool unsafe = v == b;
        for (int u : adj_list[v]) unsafe |= visited[u] ? (u != prev) : self(self, u, v);
        if (unsafe) safe[v] = false;
        return unsafe;
    };
    dfs(dfs, a, a);

    vector<int> locations;
    for (int i = 0; i < n; i++)
        if (safe[i]) locations.emplace_back(i);

    cout << locations.size() << "\n";
    for (int i : locations) cout << i << "\n";
}
