#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E, C, K, M;
    cin >> V >> E >> C >> K >> M;

    vector<vector<pair<int, int>>> adj_list(V);
    while (E--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u - 1].emplace_back(w, v - 1);
        adj_list[v - 1].emplace_back(w, u - 1);
    }

    vector<long long> dist(V, LLONG_MAX);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[0] = 0;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (dist[v] != d) continue;

        for (auto [w, u] : adj_list[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
            }
    }

    vector<long long> v;
    while (C--) {
        int f;
        cin >> f;

        if (dist[f - 1] != LLONG_MAX) v.emplace_back(dist[f - 1]);
    }
    sort(v.begin(), v.end());
    
    cout << ((min(M, K) - 1 < v.size()) ? 2 * v[min(M, K) - 1] : -1);
}
