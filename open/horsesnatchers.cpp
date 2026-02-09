#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m, t, w;
    cin >> n >> k >> m >> t >> w;
    t *= w;

    vector<bool> snatcher(n, false);
    while (k--) {
        int s;
        cin >> s;

        snatcher[s - 1] = true;
    }

    vector<vector<pair<int, int>>> adj_list_all(n), adj_list_horse(n);
    while (m--) {
        int a, b, d;
        cin >> a >> b >> d;
        a--;
        b--;

        adj_list_all[a].emplace_back(b, d);
        adj_list_all[b].emplace_back(a, d);
        if (!snatcher[a]) adj_list_horse[a].emplace_back(b, d);
        if (!snatcher[b]) adj_list_horse[b].emplace_back(a, d);
    }

    vector<long long> dist(n);
    auto dijkstra = [&](int src, const vector<vector<pair<int, int>>> &adj_list) {
        fill(dist.begin(), dist.end(), 1e18);
        dist[src] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
        return dist;
    };
    auto dist1 = dijkstra(n - 1, adj_list_all);
    if (dist1[0] <= t) {
        cout << "No horse needed!";
        exit(0);
    }

    auto speed = 1e20;
    auto dist2 = dijkstra(0, adj_list_horse);
    for (int i = 0; i < n; i++) {
        if (dist1[i] == 1e18 || dist2[i] == 1e18 || (i != n - 1 && !snatcher[i]) || t <= dist1[i]) continue;
        speed = min(speed, dist2[i] * (double) w / (t - dist1[i]));
    }

    if (speed == 1e20) cout << "Impossible";
    else cout << fixed << setprecision(6) << speed;
}
