#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s, p, d, k;
    cin >> n >> s >> p >> d >> k;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (k--) {
        int u, v, t;
        cin >> u >> v >> t;

        adj_list[u].emplace_back(v, t);
        adj_list[v].emplace_back(u, t);
    }

    auto dijkstra = [&](int src) {
        vector<long long> dist(n + 1, 1e18);
        dist[src] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [dv, v] = pq.top();
            pq.pop();

            if (dv != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > dv + w) {
                    dist[u] = dv + w;
                    pq.emplace(dist[u], u);
                }
        }
        return dist;
    };
    auto dist_s = dijkstra(s), dist_p = dijkstra(p), dist_d = dijkstra(d);

    auto t = LLONG_MAX;
    for (int v = 1; v <= n; v++) t = min(t, dist_s[v] + dist_p[v] + min(dist_p[d], dist_d[v]));
    cout << t;
}
