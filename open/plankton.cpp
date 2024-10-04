#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int t, f, fu, fn;
        cin >> t >> f >> fu >> fn;

        vector<tuple<int, int, double>> edges;
        vector<vector<int>> adj_list(f + 1);
        for (int i = 0; i < t; i++) {
            int fr, fg;
            double u;
            cin >> fr >> fg >> u;

            edges.emplace_back(fr, fg, -u);
            adj_list[fr].emplace_back(fg);
        }

        vector<double> dist(f + 1, 1e20);
        dist[fu] = 0;

        auto bellman_ford = [&]() {
            for (int i = 0; i < f; i++)
                for (auto [u, v, w] : edges) dist[v] = min(dist[v], dist[u] + w);
        };
        bellman_ford();

        vector<bool> visited(f + 1, false);
        auto dfs = [&](auto &&self, int v) -> void {
            visited[v] = true;
            for (int u : adj_list[v])
                if (!visited[u]) self(self, u);
        };
        for (auto [u, v, w] : edges)
            if (w + dist[u] < 1e20 && w + dist[u] < dist[v]) dfs(dfs, u);

        cout << (visited[fn] ? "TRUE\n" : "FALSE\n");
    }
}

