#include <bits/stdc++.h>
using namespace std;

int dijkstra(int n, vector<vector<pair<int, int>>> adj_list, bool knight) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    vector<bool> visited(n, false);
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [v, d] = pq.top();
        pq.pop();

        if (v == n - 1) return d;

        if (visited[v]) continue;

        visited[v] = true;
        for (auto &[u, w] : adj_list[v])
            if (!knight) {
                if (w <= 12) {
                    if (d % 12 + w <= 12) pq.emplace(d + w, u);
                    else pq.emplace(d + 12 - d % 12 + w, u);
                }
            } else if (!visited[u]) pq.emplace(d + w, u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n, vector<pair<int, int>>());
    while (m--) {
        int u, v, d;
        cin >> u >> v >> d;
        adj_list[u].emplace_back(v, d);
        adj_list[v].emplace_back(u, d);
    }

    int knight = dijkstra(n, adj_list, true), day = dijkstra(n, adj_list, false);
    cout << day + max(0, (day - 1) / 12) * 12 - (knight + max(0, (knight - 1) / 12) * 12);
}
