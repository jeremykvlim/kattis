#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges;
    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        edges.emplace_back(u, v);
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> dist(n + 1), prev(n + 1, -1), diff(n + 1, 0);
    vector<long long> count(2 * (n + 1), 0);
    for (int i = 1; i <= n; i++) {
        fill(dist.begin(), dist.end(), 1e9);

        queue<int> q;
        q.emplace(i);
        dist[i] = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (dist[u] == 1e9) {
                    dist[u] = dist[v] + 1;
                    prev[u] = v;
                    q.emplace(u);
                }
        }

        for (auto [u, v] : edges)
            if (dist[u] != 1e9) {
                if (dist[u] > dist[v]) swap(u, v);
                if (prev[v] != u) {
                    if (dist[u] != dist[v]) diff[v]++;
                    count[dist[u] + dist[v] + 1]++;
                }
            }

        for (int j = 1; j <= n; j++)
            if (dist[j] != 1e9) {
                count[2 * dist[j]] += diff[j] * (diff[j] - 1) / 2;
                diff[j] = 0;
            }
    }

    for (int i = 1; i <= n; i++)
        if (count[i]) {
            cout << count[i] / i;
            exit(0);
        }
}
