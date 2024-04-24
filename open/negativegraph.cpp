#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, A, B;
    cin >> n >> m >> k >> A >> B;

    vector<unordered_map<int, long long>> adj_list_pos(n), adj_list_neg(n);
    unordered_set<int> stations_neg{A, B};
    while (m--) {
        int a, b;
        long long c;
        cin >> a >> b >> c;

        adj_list_pos[a][b] = adj_list_pos[a].count(b) ? min(adj_list_pos[a][b], c) : c;
        if (c < 0) {
            stations_neg.emplace(a);
            stations_neg.emplace(b);
            adj_list_neg[a][b] = adj_list_pos[a][b];
        }
    }

    priority_queue<pair<long long, int>> pq;
    for (auto s : stations_neg) {
        pq.emplace(0, s);
        vector<bool> visited(n, false);
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (visited[u]) continue;

            visited[u] = true;
            if (stations_neg.count(u) && !adj_list_neg[s].count(u)) adj_list_neg[s][u] = -d;
            for (auto [v, w] : adj_list_pos[u])
                if (w >= 0) pq.emplace(d - w, v);
        }
    }

    unordered_map<int, long long> dist{{A, 0}};
    for (int i = 0; i <= stations_neg.size() * 4; i++) {
        bool neg_cycle = false;
        for (int v : stations_neg) {
            if (!dist.count(v)) continue;

            for (auto [u, d] : adj_list_neg[v])
                if (dist.count(u)) {
                    if (dist[u] > dist[v] + d) {
                        dist[u] = dist[v] + d;
                        if (u == B) neg_cycle = true;
                    }
                } else {
                    dist[u] = dist[v] + d;
                    if (u == B) neg_cycle = true;
                }
        }

        if (i == stations_neg.size() * 4 && neg_cycle) {
            cout << "NEGATIVE INFINITY";
            exit(0);
        }
    }

    cout << (dist.count(B) ? to_string(dist[B]) : "POSITIVE INFINITY");
}
