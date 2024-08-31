#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, p;
    cin >> n >> m >> p;

    vector<vector<pair<pair<long long, long long>, int>>> adj_list(n);
    while (m--) {
        int u, v, w;
        char c;
        cin >> u >> v >> w >> c;

        pair<long long, long long> t{c == 'I' ? 0 : w, c == 'O' ? 0 : w};
        adj_list[u].emplace_back(t, v);
        adj_list[v].emplace_back(t, u);
    }

    while (p--) {
        int s, t;
        cin >> s >> t;

        priority_queue<pair<pair<long long, long long>, int>, vector<pair<pair<long long, long long>, int>>, greater<>> pq;
        pq.push({{0, 0}, s});
        vector<pair<long long, long long>> dist(n, {LLONG_MAX, LLONG_MAX});
        dist[s] = {0, 0};
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            if (v == t) break;

            for (auto [w, u] : adj_list[v])
                if (dist[u].first > dist[v].first + w.first) {
                    dist[u].first = dist[v].first + w.first;
                    dist[u].second = dist[v].second + w.second;
                    pq.emplace(dist[u], u);
                } else if (dist[u].first == dist[v].first + w.first) {
                    if (dist[u].second > dist[v].second + w.second) {
                        dist[u].second = dist[v].second + w.second;
                        pq.emplace(dist[u], u);
                    }
                }
        }

        if (dist[t].first == LLONG_MAX) cout << "IMPOSSIBLE\n";
        else cout << dist[t].first << " " << dist[t].first + dist[t].second << "\n";
    }
}
