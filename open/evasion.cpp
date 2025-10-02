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
    vector<bool> on_p_to_s_path(n + 1, false), on_s_to_t_path(n + 1, false);
    for (int v = 1; v <= n; v++) {
        on_p_to_s_path[v] = dist_p[v] < 1e18 && dist_s[v] < 1e18 && dist_p[v] + dist_s[v] == p_to_s;
        on_s_to_t_path[v] = dist_s[v] < 1e18 && dist_t[v] < 1e18 && dist_s[v] + dist_t[v] == s_to_t;
    }

    vector<int> p_to_s_dir(m, 0), s_to_t_dir(m, 0);
    vector<pair<long long, long long>> p_to_s_by_u(m, {-1, -1}), p_to_s_by_v(m, {-1, -1}),
                                       s_to_t_by_u(m, {-1, -1}), s_to_t_by_v(m, {-1, -1});

    for (int i = 0; i < m; i++) {
        auto [u, v, w] = edges[i];

        if (dist_p[u] < 1e18 && dist_s[v] < 1e18 && dist_p[u] + w + dist_s[v] == p_to_s) {
            p_to_s_dir[i] |= 1;
            p_to_s_by_u[i] = {dist_p[u], dist_p[u] + w};
        }
        if (dist_p[v] < 1e18 && dist_s[u] < 1e18 && dist_p[v] + w + dist_s[u] == p_to_s) {
            p_to_s_dir[i] |= 2;
            p_to_s_by_v[i] = {dist_p[v], dist_p[v] + w};
        }

        if (dist_s[u] < 1e18 && dist_t[v] < 1e18 && dist_s[u] + w + dist_t[v] == s_to_t) {
            s_to_t_dir[i] |= 1;
            s_to_t_by_u[i] = {dist_s[u] + p_to_s, dist_s[u] + p_to_s + w};
        }
        if (dist_s[v] < 1e18 && dist_t[u] < 1e18 && dist_s[v] + w + dist_t[u] == s_to_t) {
            s_to_t_dir[i] |= 2;
            s_to_t_by_v[i] = {dist_s[v] + p_to_s, dist_s[v] + p_to_s + w};
        }
    }

    vector<bool> valid(n + 1, false);
    for (int v = 1; v <= n; v++)
        if (on_p_to_s_path[v])
            for (auto [u, w, i] : adj_list[v])
                if (!p_to_s_dir[i]) {
                    valid[v] = true;
                    break;
                }

    vector<array<long long, 2>> dist(n + 1, {(long long) 1e18, (long long) 1e18});
    dist[s][0] = 0;
    priority_queue<tuple<long long, int, bool>, vector<tuple<long long, int, bool>>, greater<>> pq;
    pq.emplace(0LL, s, 0);
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

        for (auto [u, w, i] : adj_list[v]) {
            int dir = (u == edges[i][0] && v == edges[i][1]) ? 2 : 1;
            if (!hidden) {
                if (p_to_s_dir[i]) {
                    bool caught = false;
                    if (p_to_s_dir[i] & 1) {
                        auto [l, r] = p_to_s_by_u[i];
                        if (dir == 1) {
                            if (d <= l && l <= d + w) caught = true;
                        } else {
                            if (l - 2 * w < d && d < r) caught = true;
                        }
                    }
                    if (caught) continue;

                    if (p_to_s_dir[i] & 2) {
                        auto [l, r] = p_to_s_by_v[i];
                        if (dir == 2) {
                            if (d <= l && l <= d + w) caught = true;
                        } else {
                            if (l - 2 * w < d && d < r) caught = true;
                        }
                    }
                    if (caught) continue;
                }

                if (on_p_to_s_path[u] && d + 2 * w == dist_p[u]) continue;
                if (dist[u][0] > d + 2 * w) {
                    dist[u][0] = d + 2 * w;
                    pq.emplace(d + 2 * w, u, 0);
                }
            } else {
                if (s_to_t_dir[i]) {
                    bool caught = false;
                    if (s_to_t_dir[i] & 1) {
                        auto [l, r] = s_to_t_by_u[i];
                        if (dir == 1) {
                            if (d <= l && l <= d + w) caught = true;
                        } else {
                            if (l - 2 * w < d && d < r) caught = true;
                        }
                    }
                    if (caught) continue;

                    if (s_to_t_dir[i] & 2) {
                        auto [l, r] = s_to_t_by_v[i];
                        if (dir == 2) {
                            if (d <= l && l <= d + w) caught = true;
                        } else {
                            if (l - 2 * w < d && d < r) caught = true;
                        }
                    }
                    if (caught) continue;
                }

                if (on_s_to_t_path[u] && d + 2 * w == dist_s[u] + p_to_s) continue;
                if (dist[u][1] > d + 2 * w) {
                    dist[u][1] = d + 2 * w;
                    pq.emplace(d + 2 * w;, u, 1);
                }
            }
        }
    }

    if (!~time) {
        auto d = min(dist[t][0], dist[t][1]);
        if (d < 1e18 && d < p_to_t) time = d;
    }
    cout << time;
}
