#include <bits/stdc++.h>
using namespace std;

vector<long long> dijkstra(int s, vector<vector<pair<int, int>>> &adj_list) {
    int n = adj_list.size();

    vector<long long> dist(n, 1e18);
    dist[s] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (dist[v] != d) continue;

        for (auto [u, w] : adj_list[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
            }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    int n, m;
    cin >> a >> b >> n >> m;

    vector<vector<pair<int, int>>> adj_list_front(n + 1), adj_list_back(n + 1);
    while (m--) {
        int u, v, t;
        cin >> u >> v >> t;

        adj_list_front[u].emplace_back(v, t);
        adj_list_back[v].emplace_back(u, t);
    }
    auto dist_front = dijkstra(1, adj_list_front), dist_back = dijkstra(n, adj_list_back);

    long long l = 0, r = 1e12, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        auto check = [&](auto d) -> bool {
            vector<bool> valid(n + 1, false);
            for (int i = 1; i <= n; i++) valid[i] = (dist_back[i] <= d);
            if (valid[1]) return true;

            queue<int> q;
            vector<bool> visited(n + 1, false);
            for (int i = 2; i <= n; i++)
                if (valid[i] && dist_front[i] + dist_back[i] <= a + d) {
                    q.emplace(i);
                    visited[i] = true;
                }

            vector<vector<pair<int, int>>> adj_list(n + 1);
            vector<int> degree(n + 1, 0);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (auto [u, w] : adj_list_front[v])
                    if (w + dist_back[u] <= d) {
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

            vector<long long> dist(n + 1, -1e18);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                if (dist_front[v] <= a + d - dist_back[v]) dist[v] = max(dist[v], a + d - dist_back[v]);
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
