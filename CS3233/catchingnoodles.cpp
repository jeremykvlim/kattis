#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

void dijkstra(int s, vector<vector<pair<int, int>>> &adj_list, vector<int> &dist, vector<long long> &count) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    count[s] = 1;
    dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (dist[v] != d) continue;

        for (auto [u, l]: adj_list[v]) {
            if (dist[u] > d + l) {
                dist[u] = d + l;
                count[u] = count[v];
                pq.emplace(d + l, u);
            } else if (dist[u] == d + l) count[u] = (count[u] + count[v]) % MODULO;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int j, b;
    cin >> j >> b;

    vector<vector<pair<int, int>>> adj_list(j);
    while (b--) {
        int u, v, l;
        cin >> u >> v >> l;

        adj_list[u].emplace_back(v, l);
        adj_list[v].emplace_back(u, l);
    }

    vector<int> dist1(j, INT_MAX), dist2(j, INT_MAX);
    vector<long long> count1(j, 0), count2(j, 0);
    dijkstra(0, adj_list, dist1, count1);
    dijkstra(j - 1, adj_list, dist2, count2);

    if (dist2[0] == INT_MAX)
        while (j--) cout << "-1 ";
    else
        for (int i = 0; i < j; i++)
            cout << ((dist1[i] + dist2[i] == dist2[0]) ? (count1[i] * count2[i]) % MODULO : -1) << " ";
}
