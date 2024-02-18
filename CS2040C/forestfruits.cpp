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

        adj_list[u - 1].emplace_back(v - 1, w);
        adj_list[v - 1].emplace_back(u - 1, w);
    }

    vector<long long> dist(V, LLONG_MAX);
    dist[0] = 0;
    priority_queue<pair<int, long long>, vector<pair<int, long long>>, greater<>> pq;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [v, d] = pq.top();
        pq.pop();

        if (dist[v] != d) continue;

        for (auto [u, w] : adj_list[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(u, d + w);
            }
    }

    vector<long long> walks;
    while (C--) {
        int f;
        cin >> f;

        if (dist[f - 1] != LLONG_MAX) walks.emplace_back(dist[f - 1]);
    }

    sort(walks.begin(), walks.end());
    cout << ((min(M, K) - 1 < walks.size()) ? 2 * walks[min(M, K) - 1] : -1);
}
