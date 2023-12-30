#include <bits/stdc++.h>
using namespace std;

void dijkstra(int s, bool t, vector<vector<pair<int, long long>>> &adj_list, vector<vector<pair<int, long long>>> &prev, vector<int> &stations) {
    vector<long long> dist(prev.size(), LLONG_MAX);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    if (!t) dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (pq.empty() && t) stations.emplace_back(v);

        for (auto [u, w] : adj_list[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
                if (!t) prev[u] = vector<pair<int, long long>>(1, {v, w});
            } else if (dist[u] == d + w && !t) prev[u].emplace_back(v, w);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, long long>>> adj_list(n);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    int s, t;
    cin >> s >> t;

    vector<vector<pair<int, long long>>> prev(n);
    vector<int> stations;
    dijkstra(s, false, adj_list, prev, stations);
    dijkstra(t, true, prev, prev, stations);

    sort(stations.begin(), stations.end());
    for (auto u : stations) cout << u << " ";
}
