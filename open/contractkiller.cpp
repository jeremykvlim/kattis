#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<vector<bool>> visited1(n, vector<bool>(n, false)), visited2(n, vector<bool>(n, false));
    vector<vector<int>> degree(n, vector<int>(n));
    queue<array<int, 3>> q;
    for (int u = 0; u < n; u++) {
        for (int &d : degree[u]) d = adj_list[u].size() + 1;
        visited1[u][u] = visited2[u][u] = true;
        q.push({u, u, 0});
        q.push({u, u, 1});
    }

    while (!q.empty()) {
        auto [u, v, turn] = q.front();
        q.pop();

        if (!turn) {
            if (!visited1[u][v]) {
                visited1[u][v] = true;
                q.push({u, v, 1});
            }

            for (int t : adj_list[v])
                if (!visited1[u][t]) {
                    visited1[u][t] = true;
                    q.push({u, t, 1});
                }
        } else {
            if (!visited2[u][v])
                if (!--degree[u][v]) {
                    visited2[u][v] = true;
                    q.push({u, v, 0});
                }

            for (int t : adj_list[u])
                if (!visited2[t][v])
                    if (!--degree[t][v]) {
                        visited2[t][v] = true;
                        q.push({t, v, 0});
                    }
        }
    }

    int sum = 0;
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++)
            if (u != v && visited2[u][v]) goto next;
        sum += u;
        next:;
    }
    cout << sum;
}
