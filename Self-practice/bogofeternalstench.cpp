#include <bits/stdc++.h>
using namespace std;

void bellman_ford(int n, vector<tuple<int, int, int>> &edges, vector<long long> &dist) {
    while (n--)
        for (auto &[u, v, s] : edges)
            if (s <= dist[v] && dist[u] < dist[v] - s) dist[u] = dist[v] - s;
}

int main() {
    int n, M;
    cin >> n >> M;

    vector<tuple<int, int, int>> edges(M);
    for (auto &[u, v, s] : edges) cin >> u >> v >> s;

    long long l = 0, r = LLONG_MAX, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        vector<long long> dist(n + 1, LLONG_MIN);
        dist[n] = m;

        bellman_ford(n, edges, dist);

        vector<bool> cycle(n + 1);
        for (int i = 0; i < n; i++)
            for (auto &[u, v, s] : edges)
                if (s <= dist[v] && dist[u] < dist[v] - s) cycle[u] = true;

        for (int i = 1; i <= n; i++)
            if (cycle[i]) dist[i] = LLONG_MAX;

        bellman_ford(n, edges, dist);

        if (dist[1] >= 0) r = m;
        else l = m;
    }

    cout << (!l ? l : r);
}
