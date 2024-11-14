#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
    }

    vector<long long> dist(n + 1, 1e18);
    dist[1] = 0;
    vector<int> visited(n + 1, 0);
    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (visited[v]++ > n) continue;

        for (auto [u, w] : adj_list[v])
            if (dist[u] > max(0LL, dist[v] + w)) {
                if (visited[u] > 1) dist[u] = (w < 0 ? 0 : max(0LL, dist[v] + w));
                else dist[u] = max(0LL, dist[v] + w);
                pq.emplace(dist[u], u);
            }
    }

    cout << dist[n];
}
