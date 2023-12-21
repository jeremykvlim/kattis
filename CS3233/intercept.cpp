#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    int s, t;
    cin >> s >> t;

    vector<vector<pair<int, long long>>> prev(n);
    vector<long long> dist(n, LLONG_MAX);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        for (auto [u, w] : adj_list[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
                prev[u] = vector<pair<int, long long>>(1, {v, w});
            } else if (dist[u] == d + w) prev[u].emplace_back(v, w);
    }

    vector<int> stations;
    fill(dist.begin(), dist.end(), LLONG_MAX);
    pq.emplace(0, t);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (pq.empty()) stations.emplace_back(v);

        for (auto [u, w] : prev[v])
            if (dist[u] > d + w) {
                dist[u] = d + w ;
                pq.emplace(d + w, u);
            }
    }

    sort(stations.begin(), stations.end());
    for (auto u : stations) cout << u << " ";
}
