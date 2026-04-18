#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, e, D;
    cin >> n >> e >> D;

    vector<vector<pair<int, int>>> adj_list_regular(n + 1), adj_list_transpose(n + 1);
    while (e--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list_regular[u].emplace_back(v, w);
        adj_list_transpose[v].emplace_back(u, w);
    }

    vector<long long> dist(n + 1, 1e18);
    dist[D] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, D);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist[v]) continue;

        for (auto [u, w] : adj_list_transpose[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
            }
    }

    vector<int> indices(n + 1, n + 1);
    indices[D] = D;
    for (int v = 1; v <= n; v++)
        if (v != D && dist[v] != 1e18)
            for (auto [u, w] : adj_list_regular[v])
                if (dist[v] == dist[u] + w) indices[v] = min(indices[v], u);

    int q;
    cin >> q;

    while (q--) {
        int x;
        cin >> x;
        cout << (indices[x] == n + 1 ? -1 : indices[x]) << "\n";
    }
}
