#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    vector<vector<pair<int, int>>> adj_list(2 * n);
    vector<int> degree(2 * n, 0);
    while (m--) {
        int a, b, w;
        cin >> a >> b >> w;

        adj_list[b + n].emplace_back(a, w);
        adj_list[b].emplace_back(a + n, w);
        degree[a]++;
        degree[a + n]++;
    }

    vector<long long> dist(2 * n, LLONG_MAX);
    dist[t] = dist[t + n] = 0;
    vector<bool> visited(2 * n, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, t);
    pq.emplace(0, t + n);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (visited[v] || v < n && --degree[v] > 0) continue;

        dist[v] = d;
        visited[v] = true;
        for (auto [u, w] : adj_list[v]) pq.emplace(d + w, u);
    }
    cout << (dist[s] == LLONG_MAX ? "infinity" : to_string(dist[s]));
}
