#include <bits/stdc++.h>
using namespace std;

pair<vector<vector<int>>, vector<vector<int>>> misra_gries(int n, const vector<pair<int, int>> &edges) {
    vector<int> degree(n + 1);
    for (auto [u, v] : edges) {
        degree[u]++;
        degree[v]++;
    }

    int delta = 0;
    for (int v = 1; v <= n; v++) delta = max(delta, degree[v] + 1);

    vector<vector<int>> edge_color(n + 1, vector<int>(n + 1, 0)), match(n + 1, vector<int>(delta + 1, 0));
    vector<int> free(n + 1, 1);
    auto set_edge_color = [&](int u, int v, int c) {
        int prev = edge_color[u][v];
        edge_color[u][v] = edge_color[v][u] = c;
        match[u][c] = v;
        match[v][c] = u;

        if (prev) {
            match[u][prev] = match[v][prev] = 0;
            free[u] = free[v] = prev;
        } else {
            for (free[u] = 1; free[u] <= delta && match[u][free[u]]; free[u]++);
            for (free[v] = 1; free[v] <= delta && match[v][free[v]]; free[v]++);
        }
        return prev;
    };

    vector<int> visited(delta + 1, 0);
    for (int e = 0, t = 1; e < edges.size(); e++, t++) {
        auto [x, y] = edges[e];
        vector<pair<int, int>> fan;
        int v = y, c0 = free[x], c = c0, d;
        while (!edge_color[x][y]) {
            d = free[v];
            fan.emplace_back(v, d);
            if (!match[v][c])
                for (int i = fan.size() - 1; ~i; i--) c = set_edge_color(x, fan[i].first, c);
            else if (!match[x][d])
                for (int i = fan.size() - 1; ~i; i--) set_edge_color(x, fan[i].first, fan[i].second);
            else if (visited[d] == t) break;
            else {
                visited[d] = t;
                v = match[x][d];
            }
        }

        if (!edge_color[x][y]) {
            while (v) {
                int u = match[v][c];
                swap(match[v][c], match[v][d]);
                if (u) edge_color[v][u] = edge_color[u][v] = d;
                if (!match[v][c]) free[v] = c;
                if (!match[v][d]) free[v] = d;
                v = u;
                swap(c, d);
            }

            if (match[x][c0]) {
                int i = fan.size() - 2;
                for (; ~i && fan[i].second != c; i--);
                for (; ~i; i--) set_edge_color(x, fan[i].first, fan[i].second);
            } else e--;
        }
    }
    return {edge_color, match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges;
    for (int u = 1; u <= n * m; u++)
        for (int v = 1; v <= n * m; v++)
            if ((u - 1) / n != (v - 1) / n) edges.emplace_back(u, v);

    auto [edge_color, match] = misra_gries(n * m, edges);
    vector<vector<pair<int, int>>> res(n * m + 2);
    for (int u = 1; u <= n * m; u++)
        for (int v = u + 1; v <= n * m; v++)
            if (edge_color[u][v]) res[edge_color[u][v]].emplace_back(u - 1, v - 1);

    for (int c = 1; c <= n * (m - 1) + 1; c++) {
        for (auto [u, v] : res[c]) cout << (char) ('A' + u / n) << u % n + 1 << "-" << (char) ('A' + v / n) << v % n + 1 << " ";
        cout << "\n";
    }
}
