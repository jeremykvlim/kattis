#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> degree(n + 1, 0);
    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int x, y;
        cin >> x >> y;

        degree[x]++;
        degree[y]++;
        adj_list[x].emplace_back(y);
        adj_list[y].emplace_back(x);
    }

    vector<int> order;
    vector<bool> visited(n + 1, false);
    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (degree[i] <= k) q.emplace(i);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (visited[v]) continue;

        visited[v] = true;
        order.emplace_back(v);
        for (int u : adj_list[v])
            if (!visited[u])
                if (--degree[u] <= k) q.emplace(u);
    }
    reverse(order.begin(), order.end());

    vector<int> color(n + 1, 0), seen(k + 2, 0);
    for (int v : order) {
        for (int u : adj_list[v]) {
            int c = color[u];
            if (c) seen[c] = v;
        }

        for (int c = 1; c <= adj_list[v].size() + 1; c++)
            if (seen[c] != v) {
                color[v] = c;
                break;
            }
    }
    for (int i = 1; i <= n; i++) cout << color[i] << " ";
}
