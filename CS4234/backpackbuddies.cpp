#include <bits/stdc++.h>
using namespace std;

int dijkstra(int n, vector<vector<pair<int, int>>> adj_list, bool knight) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    vector<bool> visited(n, false);
    pq.emplace(0, 0);
    while (!pq.empty()) {
        int v = pq.top().second, d = pq.top().first;
        pq.pop();
        if (v == n - 1) return d;
        if (visited[v]) continue;

        visited[v] = true;
        for (auto &neighbour: adj_list[v])
            if (!knight) {
                if (neighbour.second <= 12) {
                    if (d % 12 + neighbour.second <= 12) pq.emplace(d + neighbour.second, neighbour.first);
                    else pq.emplace(d + 12 - d % 12 + neighbour.second, neighbour.first);
                }
            } else if (!visited[neighbour.first]) pq.emplace(d + neighbour.second, neighbour.first);
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
