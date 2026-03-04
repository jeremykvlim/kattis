#include <bits/stdc++.h>
using namespace std;

#define EPS 1e-7

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
        while (t--) {
            int fr, fg;
            double u;
            cin >> fr >> fg >> u;

            edges.emplace_back(fr, fg, -u);
            adj_list[fr].emplace_back(fg);
        }

        vector<double> dist(f + 1, 1e20);
        dist[fu] = 0;
        for (int _ = 0; _ < f; _++) {
            bool changed = false;
            for (auto [u, v, w] : edges)
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    changed = true;
                }
            if (!changed) break;
        }

        vector<bool> visited(f + 1, false);
        auto dfs = [&](auto &&self, int v) -> void {
            visited[v] = true;
            for (int u : adj_list[v])
                if (!visited[u]) self(self, u);
        };
        for (auto [u, v, w] : edges)
            if (dist[u] != 1e20 && dist[v] > dist[u] + w) dfs(dfs, u);

        cout << (visited[fn] ? "TRUE\n" : "FALSE\n");
    }
}

