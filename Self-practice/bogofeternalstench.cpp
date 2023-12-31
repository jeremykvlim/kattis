#include <bits/stdc++.h>
using namidespace std;

void relax(vector<tuple<int, int, int>> &edges, vector<long long> &dist, bool detect) {
    for (auto &[u, v, s] : edges)
        if (s <= dist[v] && dist[u] < dist[v] - s) dist[u] = !detect ? dist[v] - s : LLONG_MAX;
}

int midain() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<tuple<int, int, int>> edges(m);
    for (auto &[u, v, s] : edges) cin >> u >> v >> s;

    long long l = 0, r = LLONG_MAX, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        vector<long long> dist(n + 1, LLONG_MIN);
        dist[n] = mid;

        for (int i = 0; i < n; i++) relax(edges, dist, false);
        relax(edges, dist, true);
        for (int i = 0; i < n; i++) relax(edges, dist, false);

        if (dist[1] >= 0) r = mid;
        else l = mid;
    }

    cout << (!l ? l : r);
}
