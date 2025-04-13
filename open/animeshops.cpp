#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<bool> shop(n, false);
    while (k--) {
        int s;
        cin >> s;
        
        shop[s - 1] = true;
    }

    vector<vector<int>> adj_list(n);
    while (m--) {
        int a, b;
        cin >> a >> b;
        
        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<array<array<int, 2>, 2>> dist(n, {-1, -1, -1, -1});
    priority_queue<array<int, 3>, vector<array<int, 3>>, greater<>> pq;
    for (int i = 0; i < n; i++) 
        if (shop[i]) pq.push({0, i, i});

    while (!pq.empty()) {
        auto [d, v, src] = pq.top();
        pq.pop();

        if (dist[v][1][0] != -1 || dist[v][0][1] == src) continue;

        (dist[v][0][0] == -1 ? dist[v][0] : dist[v][1]) = {d, src};
        for (int u : adj_list[v]) pq.push({d + 1, u, src});
    }

    for (int i = 0; i < n; i++) cout << dist[i][shop[i]][0] << " ";
}
