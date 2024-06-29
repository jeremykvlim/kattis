#include <bits/stdc++.h>
using namespace std;

bool relax(vector<array<int, 3>> &edges, vector<int> &dist, bool detect = false) {
    bool change = false;
    for (auto &[u, v, e] : edges)
        if (-e <= dist[u] && dist[v] < dist[u] + e) {
            dist[v] = !detect ? dist[u] + e : 1e6;
            change = true;
        }

    return change;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n != -1) {
        vector<array<int, 3>> edges;

        for (int u = 1; u <= n; u++) {
            int e, doorways;
            cin >> e >> doorways;

            while (doorways--) {
                int v;
                cin >> v;

                edges.push_back({u, v, e});
            }
        }

        vector<int> dist(n + 1, INT_MIN);
        dist[1] = 100;

        auto bellman_ford = [&]() {
            for (int i = 0; i < n; i++)
                if (!relax(edges, dist)) break;
        };

        bellman_ford();
        relax(edges, dist, true);
        bellman_ford();

        cout << (dist[n] > 0 ? "winnable\n" : "hopeless\n");
    }
}
