#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    vector<int> degree(n, 0);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
        degree[u - 1]++;
        degree[v - 1]++;
    }

    vector<int> path;
    vector<bool> visited(n, false);
    queue<int> q;
    priority_queue<pair<int, int>> pq;
    for (int i = 0; i < n; i++) pq.emplace(degree[i], i);
    while (path.size() < n) {
        int v = -1;
        while (v == -1) {
            if (pq.empty()) {
                cout << -1;
                exit(0);
            }

            auto [d, i] = pq.top();
            pq.pop();

            if (degree[i] == d) v = i;
        }

        visited[v] = true;
        path.emplace_back(v);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            pq.emplace(degree[u], u);
        }

        for (int u : adj_list[v])
            if (!visited[u]) {
                degree[u]--;
                q.emplace(u);
            }
    }

    for (int v : path) cout << v + 1 << " ";
}
