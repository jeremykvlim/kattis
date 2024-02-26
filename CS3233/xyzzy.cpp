#include <bits/stdc++.h>
using namespace std;

void relax(vector<tuple<int, int, int>> &edges, vector<int> &dist, bool detect = false) {
    for (auto &[u, v, e] : edges)
        if (-e <= dist[u] && dist[v] < dist[u] + e) dist[v] = !detect ? dist[u] + e : 1e6;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n != -1) {
        vector<tuple<int, int, int>> edges;

        for (int u = 1; u <= n; u++) {
            int e, doorways;
            cin >> e >> doorways;

            while (doorways--) {
                int v;
                cin >> v;

                edges.emplace_back(u, v, e);
            }
        }

        vector<int> dist(n + 1, INT_MIN);
        dist[1] = 100;

        for (int i = 0; i < n; i++) relax(edges, dist);
        relax(edges, dist, true);
        for (int i = 0; i < n; i++) relax(edges, dist);

        cout << (dist[n] > 0 ? "winnable\n" : "hopeless\n");
    }
}
