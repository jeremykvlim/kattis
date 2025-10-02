#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n);
    while (m--) {
        int u, v, d;
        cin >> u >> v >> d;

        adj_list[u].emplace_back(v, d);
        adj_list[v].emplace_back(u, d);
    }

    vector<bool> visited(n, false);
    auto dijkstra = [&](bool knight = true) {
        fill(visited.begin(), visited.end(), false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.emplace(0, 0);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (v == n - 1) return d;

            if (visited[v]) continue;

            visited[v] = true;
            for (auto [u, w] : adj_list[v])
                if (!knight) {
                    if (w <= 12) {
                        if (d % 12 + w <= 12) pq.emplace(d + w, u);
                        else pq.emplace(d + 12 - d % 12 + w, u);
                    }
                } else if (!visited[u]) pq.emplace(d + w, u);
        }
    };

    int knight = dijkstra(), day = dijkstra(false);
    cout << day + max(0, (day - 1) / 12) * 12 - (knight + max(0, (knight - 1) / 12) * 12);
}