#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E, C, K, M;
    cin >> V >> E >> C >> K >> M;

    vector<vector<pair<int, int>>> adj_list(V + 1);
    vector<long long> dist(V + 1, LLONG_MAX);

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    priority_queue<pair<int, long long>> pq;
    dist[1] = 0;
    pq.emplace(1, 0);
    
    while (!pq.empty()) {
        auto v = pq.top();
        pq.pop();

        if (-v.second != dist[v.first]) continue;

        for (auto u : adj_list[v.first])
            if (dist[u.first] > dist[v.first] + u.second) {
                dist[u.first] = dist[v.first] + u.second;
                pq.emplace(u.first, -dist[u.first]);
            }
    }

    vector<long long> v;
    for (int i = 0; i < C; i++) {
        int f;
        cin >> f;
        if (dist[f] != LLONG_MAX) v.emplace_back(dist[f]);
    }

    sort(v.begin(), v.end());
    cout << ((min(M, K) - 1 < v.size()) ? 2 * v[min(M, K) - 1] : -1) << '\n';
}
