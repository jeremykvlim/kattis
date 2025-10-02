#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, a;
    cin >> m >> n >> a;

    int count = 0;
    vector<vector<int>> stations(n), node(n), station_nodes(m);
    for (int i = 0; i < n; i++) {
        int size;
        cin >> size;

        stations[i].resize(size);
        node[i].resize(size);
        for (int j = 0; j < size; j++) {
            cin >> stations[i][j];
            stations[i][j]--;

            node[i][j] = count++;
            station_nodes[stations[i][j]].emplace_back(node[i][j]);
        }
    }

    vector<vector<int>> adj_list(count);
    for (int i = 0; i < n; i++)
        for (int j = 0; j + 1 < stations[i].size(); j++) {
            int u = node[i][j], v = node[i][j + 1];
            adj_list[u].emplace_back(v);
            adj_list[v].emplace_back(u);
        }

    vector<vector<int>> switch_lines(count);
    for (auto nodes : station_nodes)
        for (int u : nodes)
            for (int v : nodes)
                if (u != v) switch_lines[v].emplace_back(u);

    vector<vector<int>> dist(m, vector<int>(count, INT_MAX));
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < m; i++) {
        if (!i)
            for (int v : station_nodes[i]) {
                dist[i][v] = 0;
                pq.emplace(0, v);
            }
        else
            for (int v = 0; v < count; v++) {
                int d = INT_MAX;
                for (int u : switch_lines[v]) d = min(d, dist[i - 1][u]);

                if (d != INT_MAX) {
                    dist[i][v] = d;
                    pq.emplace(d, v);
                }
            }

        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[i][v]) continue;

            for (int u : adj_list[v])
                if (dist[i][u] > d + 1) {
                    dist[i][u] = d + 1;
                    pq.emplace(d + 1, u);
                }
        }
    }

    vector<long long> cost(m, LLONG_MAX);
    for (int i = 0; i < m; i++) {
        int d = INT_MAX;
        for (int v : station_nodes[m - 1]) d = min(d, dist[i][v]);

        if (d != INT_MAX) cost[i] = (long long) d * a;
    }

    int t;
    cin >> t;

    while (t--) {
        int b;
        cin >> b;

        auto time = LLONG_MAX;
        for (int i = 0; i < m; i++)
            if (cost[i] != LLONG_MAX) time = min(time, cost[i] + i * b);

        cout << time << "\n";
    }
}
