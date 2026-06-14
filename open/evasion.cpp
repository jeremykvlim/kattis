#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t, p;
    cin >> n >> m >> s >> t >> p;

    vector<array<int, 3>> edges(m);
    vector<vector<array<int, 3>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;

        edges[i] = {u, v, w};
        adj_list[u].push_back({v, w, i});
        adj_list[v].push_back({u, w, i});
    }

    auto dijkstra = [&](int src) {
        vector<long long> dist(n + 1, 1e18);
        dist[src] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w, i] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
        return dist;
    };
    auto dist_s = dijkstra(s), dist_t = dijkstra(t), dist_p = dijkstra(p);

    auto p_to_s = dist_p[s], s_to_t = dist_s[t], p_to_t = (p_to_s == 1e18 || s_to_t == 1e18) ? (long long) 1e18 : p_to_s + s_to_t;
    auto vertex_on_path = [&](int v, auto &dist1, auto &dist2, long long total) {
        return dist1[v] < 1e18 && dist2[v] < 1e18 && dist1[v] + dist2[v] == total;
    };

    auto edge_on_path = [&](int u, int v, long long w, const vector<long long> &dist1, const vector<long long> &dist2, long long total) {
        return dist1[u] < 1e18 && dist2[v] < 1e18 && dist1[u] + w + dist2[v] == total;
    };

    vector<bool> valid(n + 1, false);
    for (int v = 1; v <= n; v++)
        if (vertex_on_path(v, dist_p, dist_s, p_to_s))
            for (auto [u, w, i] : adj_list[v])
                if (!edge_on_path(u, v, w, dist_p, dist_s, p_to_s) && !edge_on_path(v, u, w, dist_p, dist_s, p_to_s)) {
                    valid[v] = true;
                    break;
                }

    vector<array<long long, 2>> dist(n + 1, {(long long) 1e18, (long long) 1e18});
    dist[s][0] = 0;
    priority_queue<tuple<long long, int, bool>, vector<tuple<long long, int, bool>>, greater<>> pq;
    pq.emplace(0, s, false);
    auto time = -1LL;
    while (!pq.empty()) {
        auto [d, v, hidden] = pq.top();
        pq.pop();

        if (d != dist[v][hidden]) continue;

        if (v == t) {
            if (d < p_to_t) time = d;
            break;
        }

        if (!hidden && valid[v])
            if (dist[v][1] > max(d, dist_p[v])) {
                dist[v][1] = max(d, dist_p[v]);
                pq.emplace(max(d, dist_p[v]), v, 1);
            }

        auto caught = [&](int u, long long w) {
            auto &dist1 = hidden ? dist_s : dist_p, &dist2 = hidden ? dist_t : dist_s;
            auto total = hidden ? s_to_t : p_to_s, offset = hidden ? p_to_s : 0;
            if (total == 1e18 || offset == 1e18) return false;

            if (edge_on_path(v, u, w, dist1, dist2, total) && d <= offset + dist1[v] && offset + dist1[v] <= d + w) return true;
            if (edge_on_path(u, v, w, dist1, dist2, total) && offset + dist1[u] - 2 * w < d && d < offset + dist1[u] + w) return true;
            return vertex_on_path(u, dist1, dist2, total) && d + 2 * w == offset + dist1[u];
        };

        for (auto [u, w, i] : adj_list[v])
            if (!caught(u, w))
                if (dist[u][hidden] > d + 2 * w) {
                    dist[u][hidden] = d + 2 * w;
                    pq.emplace(d + 2 * w, u, hidden);
                }
    }

    if (!~time) {
        auto d = min(dist[t][0], dist[t][1]);
        if (d < 1e18 && d < p_to_t) time = d;
    }
    cout << time;
}
