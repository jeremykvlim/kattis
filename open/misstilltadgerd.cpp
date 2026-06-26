#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    n *= 2;
    s--;
    t--;

    vector<array<int, 3>> edges;
    vector<vector<int>> adj_list_regular(n), adj_list_transpose(n);
    while (m--) {
        int a, b;
        string w;
        cin >> a >> b >> w;
        a--;
        b--;

        bool neg = w[0] == '-';
        int x = stoi(w.substr(w.find('^') + 1));
        for (int i = 0; i < 2; i++) {
            int u = 2 * a + i, v = 2 * b + (i ^ neg);
            edges.push_back({u, v, x});
            adj_list_regular[u].emplace_back(v);
            adj_list_transpose[v].emplace_back(u);
        }
    }

    auto bfs = [&](int src, const vector<vector<int>> &adj_list) {
        vector<bool> visited(n, false);
        visited[src] = true;
        queue<int> q;
        q.emplace(src);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (!visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
        }
        return visited;
    };
    auto visited1 = bfs(2 * s, adj_list_regular);

    vector<int> dist(n);
    auto bellman_ford = [&](int target, int sgn) -> pair<int, int> {
        if (!visited1[target]) return {0, 0};

        auto visited2 = bfs(target, adj_list_transpose);
        vector<array<int, 3>> valid;
        for (auto [u, v, w] : edges)
            if (visited1[u] && visited2[v]) valid.push_back({u, v, sgn * w});

        fill(dist.begin(), dist.end(), 1e9);
        dist[2 * s] = 0;
        for (int _ = 0; _ < n - 1; _++) {
            bool change = false;

            for (auto [u, v, w] : valid)
                if (dist[u] != 1e9 && dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    change = true;
                }

            if (!change) break;
        }

        for (auto [u, v, w] : valid)
            if (dist[u] != 1e9 && dist[v] > dist[u] + w) return {-1, 0};

        return {1, sgn * dist[target]};
    };
    auto [type1, x1] = bellman_ford(2 * t + 1, -1);
    if (type1 == -1) cout << "-infinity";
    else if (type1 == 1) cout << "-2^" << x1;
    else {
        auto [type2, x2] = bellman_ford(2 * t, 1);
        if (type2 == -1) cout << "epsilon";
        else if (type2 == 1) cout << "2^" << x2;
        else cout << "unreachable";
    }
}
