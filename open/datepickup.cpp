#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    int n, m;
    cin >> a >> b >> n >> m;

    vector<vector<pair<int, int>>> adj_list_regular(n + 1), adj_list_transpose(n + 1);
    while (m--) {
        int u, v, t;
        cin >> u >> v >> t;

        adj_list_regular[u].emplace_back(v, t);
        adj_list_transpose[v].emplace_back(u, t);
    }

    vector<long long> dist(n + 1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    auto dijkstra = [&](int s, auto &adj_list) {
        fill(dist.begin(), dist.end(), 1e18);
        dist[s] = 0;
        pq.emplace(0, s);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
        return dist;
    };
    auto dist_regular = dijkstra(1, adj_list_regular), dist_transpose = dijkstra(n, adj_list_transpose);

    queue<int> q;
    vector<bool> valid(n + 1), visited(n + 1);
    long long l = 0, r = 1e12, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        auto check = [&](auto d) -> bool {
            fill(valid.begin(), valid.end(), false);
            for (int i = 1; i <= n; i++) valid[i] = (dist_transpose[i] <= d);
            if (valid[1]) return true;

            fill(visited.begin(), visited.end(), false);
            for (int i = 2; i <= n; i++)
                if (valid[i] && dist_regular[i] + dist_transpose[i] <= a + d) {
                    q.emplace(i);
                    visited[i] = true;
                }

            vector<vector<pair<int, int>>> adj_list(n + 1);
            vector<int> degree(n + 1, 0);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (auto [u, w] : adj_list_regular[v])
                    if (w + dist_transpose[u] <= d) {
                        if (!visited[u]) {
                            q.emplace(u);
                            visited[u] = true;
                        }
                        adj_list[v].emplace_back(u, w);
                        degree[u]++;
                    }
            }

            for (int i = 1; i <= n; i++)
                if (visited[i] && !degree[i]) q.emplace(i);

            fill(dist.begin(), dist.end(), -1e18);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                if (dist_regular[v] <= a + d - dist_transpose[v]) dist[v] = max(dist[v], a + d - dist_transpose[v]);
                if (dist[v] >= b) return true;

                for (auto [u, w] : adj_list[v]) {
                    dist[u] = max(dist[u], dist[v] + w);
                    if (!--degree[u]) q.emplace(u);
                }
            }

            for (int i = 1; i <= n; i++)
                if (visited[i] && degree[i]) return true;
            return false;
        };

        if (check(mid)) r = mid;
        else l = mid;
    }

    cout << (!l ? l : r);
}
