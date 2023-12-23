#include <bits/stdc++.h>
using namespace std;

void bellman_ford(int n, vector<tuple<int, int, int>> &edges, vector<long long> &dist, vector<bool> &cycle, bool detect) {
    while (n--)
        for (auto &[u, v, s] : edges)
            if (s <= dist[v] && dist[u] < dist[v] - s) {
                if (!detect) dist[u] = dist[v] - s;
                else cycle[u] = true;
            }
}

int main() {
    int n, M;
    cin >> n >> M;

    vector<tuple<int, int, int>> edges(M);
    for (auto &[u, v, s] : edges) cin >> u >> v >> s;

    long long l = 0, r = LLONG_MAX, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        vector<bool> cycle(n + 1);
        vector<long long> dist(n + 1, LLONG_MIN);
        dist[n] = m;

        bellman_ford(n, edges, dist, cycle, false);
        bellman_ford(n, edges, dist, cycle, true);
        transform(dist.begin() + 1, dist.end(), cycle.begin() + 1, dist.begin() + 1, [&](long long d, bool c) {return c ? LLONG_MAX : d;});
        bellman_ford(n, edges, dist, cycle, false);

        if (dist[1] >= 0) r = m;
        else l = m;
    }

    cout << (!l ? l : r);
}
