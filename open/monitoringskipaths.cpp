#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    cin >> n >> k >> m;

    vector<vector<int>> adj_list(n + 1);
    vector<int> degree(n + 1, 0);
    while (k--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        degree[v] = degree[v] + 1;
    }

    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (!degree[i]) q.emplace(i);

    vector<int> depth(n + 1, 0), order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        order.emplace_back(v);
        for (int u : adj_list[v]) {
            depth[u] = depth[v] + 1;
            q.emplace(u);
        }
    }

    vector<int> deepest(n + 1, -1);
    while (m--) {
        int s, t;
        cin >> s >> t;

        deepest[t] = max(deepest[t], depth[s]);
    }

    int count = 0;
    vector<int> extra(n + 1, -1);
    reverse(order.begin(), order.end());
    for (int v : order) {
        for (int u : adj_list[v]) deepest[v] = max(deepest[v], extra[u]);

        if (~deepest[v] && deepest[v] == depth[v]) count++;
        else extra[v] = deepest[v];
    }
    cout << count;
}
