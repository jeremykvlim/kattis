#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, A, B;
    cin >> n >> m >> k >> A >> B;

    vector<vector<pair<int, long long>>> adj_list_pos(n);
    vector<int> stations_neg{A, B};
    vector<tuple<int, int, long long>> neg;
    while (m--) {
        int a, b;
        long long c;
        cin >> a >> b >> c;

        if (c >= 0) adj_list_pos[a].emplace_back(b, c);
        else {
            neg.emplace_back(a, b, c);
            stations_neg.emplace_back(a);
            stations_neg.emplace_back(b);
        }
    }
    sort(stations_neg.begin(), stations_neg.end());
    stations_neg.erase(unique(stations_neg.begin(), stations_neg.end()), stations_neg.end());

    int s = stations_neg.size();
    vector<int> indices(n, -1);
    for (int i = 0; i < s; i++) indices[stations_neg[i]] = i;

    vector<vector<int>> adj_list(s);
    vector<tuple<int, int, long long>> edges;
    vector<long long> dist_pos(n);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int i = 0; i < s; i++) {
        int src = stations_neg[i];
        fill(dist_pos.begin(), dist_pos.end(), 1e18);
        dist_pos[src] = 0;
        pq.emplace(0, src);

        vector<long long> weight(s, 1e18);
        int count = 0;
        while (!pq.empty() && count < s) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist_pos[v]) continue;

            if (indices[v] != -1 && weight[indices[v]] == 1e18) {
                weight[indices[v]] = d;
                count++;
            }

            for (auto [u, w] : adj_list_pos[v])
                if (dist_pos[u] > d + w) {
                    dist_pos[u] = d + w;
                    pq.emplace(dist_pos[u], u);
                }
        }

        for (int j = 0; j < s; j++)
            if (i != j && weight[j] != 1e18) {
                edges.emplace_back(i, j, weight[j]);
                adj_list[i].emplace_back(j);
            }
    }

    for (auto [a, b, c] : neg) {
        int u = indices[a], v = indices[b];
        edges.emplace_back(u, v, c);
        adj_list[u].emplace_back(v);
    }

    vector<long long> dist(s, 1e18);
    dist[indices[A]] = 0;
    for (int _ = 0; _ < s - 1; _++) {
        bool changed = false;
        for (auto [u, v, w] : edges)
            if (dist[u] != 1e18 && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        if (!changed) break;
    }

    vector<bool> neg_cycle(s, false);
    for (auto [u, v, w] : edges)
        if (dist[u] != 1e18 && dist[v] > dist[u] + w) neg_cycle[v] = true;

    queue<int> q;
    for (int i = 0; i < s; i++)
        if (neg_cycle[i]) q.emplace(i);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list[v])
            if (!neg_cycle[u]) {
                neg_cycle[u] = true;
                q.emplace(u);
            }
    }

    if (neg_cycle[indices[B]]) {
        cout << "NEGATIVE INFINITY";
        exit(0);
    }
    if (dist[indices[B]] == 1e18) {
        cout << "POSITIVE INFINITY";
        exit(0);
    }
    cout << dist[indices[B]];
}
