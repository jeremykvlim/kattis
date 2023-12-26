#include <bits/stdc++.h>
using namespace std;

void relax(vector<tuple<int, int, int>> &edges, vector<long long> &dist, bool detect) {
    for (auto &[u, v, s] : edges)
        if (s <= dist[v] && dist[u] < dist[v] - s) dist[u] = !detect ? dist[v] - s : LLONG_MAX;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, M;
    cin >> n >> M;

    vector<tuple<int, int, int>> edges(M);
    for (auto &[u, v, s] : edges) cin >> u >> v >> s;

    long long l = 0, r = LLONG_MAX, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        vector<long long> dist(n + 1, LLONG_MIN);
        dist[n] = m;

        for (int i = 0; i < n; i++) relax(edges, dist, false);
        relax(edges, dist, true);
        for (int i = 0; i < n; i++) relax(edges, dist, false);

        if (dist[1] >= 0) r = m;
        else l = m;
    }

    cout << (!l ? l : r);
}
