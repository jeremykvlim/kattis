#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> costs;
    vector<vector<pair<int, int>>> adj_list(n);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        costs.emplace_back(w);
        adj_list[u].emplace_back(v, w);
    }
    costs.emplace_back(0);
    sort(costs.begin(), costs.end());
    costs.erase(unique(costs.begin(), costs.end()), costs.end());

    vector<int> depth(n, -1);
    depth[0] = 0;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, w] : adj_list[v])
            if (!~depth[u]) {
                depth[u] = depth[v] + 1;
                q.emplace(u);
            }
    }

    if (depth[n - 1] <= k) {
        cout << 0;
        exit(0);
    }

    int pay = INT_MAX;
    vector<int> dist(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    for (int c : costs) {
        fill(dist.begin(), dist.end(), 1e9);
        dist[0] = 0;
        pq.emplace(0, 0);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + max(w, c)) {
                    dist[u] = d + max(w, c);
                    pq.emplace(d + max(w, c), u);
                }
        }

        pay = min(pay, dist[n - 1] - k * c);
    }
    cout << pay;
}