#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (auto &[u, v] : edges) {
        int w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<long long> dist(n + 1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    auto dijkstra = [&](int src) {
        fill(dist.begin(), dist.end(), 1e18);
        dist[src] = 0;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(dist[u], u);
                }
        }
        return dist;
    };
    auto dist_s = dijkstra(s), dist_t = dijkstra(t);

    auto d = dist_s[t];
    for (auto [u, v] : edges) d = min({d, dist_s[u] + dist_t[v], dist_s[v] + dist_t[u]});
    if (d == 1e18) cout << "IMPOSSIBLE";
    else cout << d;
}