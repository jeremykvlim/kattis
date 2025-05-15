#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> degree(n);
    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        degree[u - 1]++;
        adj_list[v - 1].emplace_back(u - 1);
    }

    queue<int> q;
    vector<int> dist(n, 1e9);
    for (int i = 0; i < n; i++)
        if (!degree[i]) {
            dist[i] = 0;
            q.emplace(i);
        }

    vector<bool> visited(n, false);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (visited[v]) continue;
      
        visited[v] = true;
        for (int u : adj_list[v]) {
            degree[u]--;
            if (0 <= degree[u] && degree[u] <= 1 && dist[u] > dist[v] + 1) {
                dist[u] = dist[v] + 1;
                q.emplace(u);
            }
        }
    }

    for (int d : dist) cout << (d == 1e9 ? -1 : d) << " ";
}
