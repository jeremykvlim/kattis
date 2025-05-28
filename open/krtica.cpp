#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<vector<int>> prev(2, vector<int>(n + 1, -1)), depth(2, vector<int>(n + 1, -1)), diameter(2, vector<int>(n + 1, -1));
    prev[0][1] = 0;
    queue<int> q;
    q.emplace(1);
    auto bfs = [&](int k) {
        vector<int> order;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            order.emplace_back(v);

            for (int u : adj_list[v])
                if (u != prev[k][v]) {
                    q.emplace(u);
                    prev[k][u] = v;
                }
        }
        return order;
    };
    int end0 = bfs(0).back(), end1 = -1;
    
    for (int k = 0; k < 2; k++) {
        q.emplace(!k ? end0 : end1);
        prev[k][q.front()] = 0;
        auto order = bfs(k);
        if (!k) end1 = order.back();

        reverse(order.begin(), order.end());
        for (int v : order) {
            int d = 0, d1 = 0, d2 = 0;
            for (int u : adj_list[v])
                if (u != prev[k][v]) {
                    d = max(d, diameter[k][u]);
                    int d3 = depth[k][u] + 1;
                    if (d1 < d3) {
                        d2 = d1;
                        d1 = d3;
                    } else d2 = max(d2, d3);
                }
            depth[k][v] = d1;
            diameter[k][v] = max(d, d1 + d2);
        }
    }

    int dist = 1e9, x = -1, y = -1;
    for (int v = end1; v != end0;) {
        int u = prev[0][v], d = max({diameter[0][v], diameter[1][u], (diameter[0][v] + 1) / 2 + (diameter[1][u] + 1) / 2 + 1});
        if (dist > d) {
            dist = d;
            x = v;
            y = u;
        }
        v = u;
    }
    cout << dist << "\n" << x << " " << y << "\n";

    auto walk = [&](int z, int k) {
        int d = (diameter[k][z] + 1) / 2;
        while (depth[k][z] > d) z = prev[k ^ 1][z];
        cout << z << " ";
    };
    walk(x, 0);
    walk(y, 1);
}
