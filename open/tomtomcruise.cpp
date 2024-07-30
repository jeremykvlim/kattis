#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> weights(n);
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
    for (int i = 0; i < n; i++) {
        cin >> weights[i];

        pq.emplace(weights[i], pair<int, int>{i, i});
    }

    vector<vector<pair<int, int>>> adj_list(n);
    while (m--) {
        int f, t, w;
        cin >> f >> t >> w;

        adj_list[f].emplace_back(t, w);
        adj_list[t].emplace_back(f, w);
    }

    vector<int> visited(n, -1);
    while (!pq.empty()) {
        auto [d, p] = pq.top();
        auto [s, v] = p;
        pq.pop();

        if (v == -1) {
            cout << d;
            exit(0);
        }

        if (visited[v] == -2 || visited[v] == s) continue;
        visited[v] = visited[v] == -1 ? s : -2;

        if (s != v) pq.emplace(d + weights[v], pair<int, int>{-1, -1});
        
        for (auto [u, w] : adj_list[v]) pq.emplace(d + w, pair<int, int>{s, u});
    }
}
