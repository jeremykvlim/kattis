#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, d;
    cin >> n >> m >> d;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int u, v, h;
        cin >> u >> v >> h;

        adj_list[u].emplace_back(v, h);
    }

    vector<int> order;
    vector<long long> dist(n + 1, 1e18);
    dist[1] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [dv, v] = pq.top();
        pq.pop();

        if (dv > dist[v]) continue;

        order.emplace_back(v);
        for (auto [u, w]: adj_list[v])
            if (dist[u] > dv + w) {
                dist[u] = dv + w;
                pq.emplace(dv + w, u);
            }
    }

    vector<long long> t(d);
    for (auto &ti : t) cin >> ti;
    sort(t.begin(), t.end());

    vector<int> remaining(n + 1, 0), paths(n + 1, 0), prev(n + 1);
    remaining[1] = !t[0];
    paths[1] = 1;
    for (int v : order)
        if (paths[v]) {
            int r = remaining[v];
            auto time = r < d ? t[r] : 1e18;
            for (auto [u, w] : adj_list[v])
                if (dist[v] + w == dist[u] && dist[u] <= time) {
                    remaining[u] = r + (dist[u] == time);
                    paths[u] = min(2, paths[u] + paths[v]);
                    prev[u] = v;
                }
        }

    if (remaining[n] != d) {
        cout << 0;
        exit(0);
    }

    if (paths[n] > 1) {
        cout << 1;
        exit(0);
    }

    vector<int> path;
    for (int v = n; v; v = prev[v]) path.emplace_back(v);
    reverse(path.begin(), path.end());

    cout << path.size() << "\n";
    for (int v : path) cout << v << "\n";
}
