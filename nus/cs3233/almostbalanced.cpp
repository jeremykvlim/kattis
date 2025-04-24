#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < n; i++) {
        adj_list[i].emplace_back(i + 1, 1);
        adj_list[i + 1].emplace_back(i, 1);
    }

    while (m--) {
        int t, l, r, v;
        cin >> t >> l >> r >> v;

        if (!t) adj_list[l - 1].emplace_back(r, v - (v & 1));
        else adj_list[r].emplace_back(l - 1, v - (v & 1));
    }

    vector<int> dist(n + 1, 1e9);
    dist[0] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();
        
        if (d != dist[v]) continue;
        
        for (auto [u, w] : adj_list[v]) 
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(dist[u], u);
            }
    }

    string s(n, '0');
    for (int i = 1; i <= n; i++) s[i - 1] += dist[i] < dist[i - 1];
    cout << s;
}
