#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> c(n);
    for (int &ci : c) cin >> ci;

    vector<vector<int>> adj_list_regular(n), adj_list_transpose(n);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list_regular[a].emplace_back(b);
        adj_list_transpose[b].emplace_back(a);
    }

    vector<bool> visited(n, false);
    vector<int> order;
    auto dfs1 = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : adj_list_regular[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    for (int v = 0; v < n; v++)
        if (!visited[v]) dfs1(dfs1, v);

    int sccs = 0;
    vector<int> component(n, -1);
    vector<long long> sum;
    auto dfs2 = [&](auto &&self, int v, int id) -> void {
        sum[component[v] = id] += c[v];
        for (int u : adj_list_transpose[v])
            if (!~component[u]) self(self, u, id);
    };

    reverse(order.begin(), order.end());
    for (int v : order)
        if (!~component[v]) {
            sum.emplace_back(0);
            dfs2(dfs2, v, sccs++);
        }

    vector<vector<int>> dag(sccs);
    for (int v = 0; v < n; v++)
        for (int u : adj_list_regular[v])
            if (component[v] != component[u]) dag[component[v]].emplace_back(component[u]);

    for (int v = 0; v < sccs; v++) {
        sort(dag[v].begin(), dag[v].end());
        dag[v].erase(unique(dag[v].begin(), dag[v].end()), dag[v].end());
    }

    vector<int> degree(sccs, 0);
    for (int v = 0; v < sccs; v++)
        for (int u : dag[v]) degree[u]++;

    queue<int> q;
    for (int v = 0; v < sccs; v++)
        if (!degree[v]) q.emplace(v);

    order.clear();
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        order.emplace_back(v);
        for (int u : dag[v])
            if (!--degree[u]) q.emplace(u);
    }

    vector<long long> dp(sccs, 0);
    dp[component[0]] = sum[component[0]];
    for (int v : order)
        if (dp[v])
            for (int u : dag[v]) dp[u] = max(dp[u], dp[v] + sum[u]);
    cout << dp[component[n - 1]];
}
