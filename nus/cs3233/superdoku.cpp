#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> grid(n + 1, vector<int>(n + 1));
    vector<vector<bool>> row(n + 1, vector<bool>(n + 1)), col(n + 1, vector<bool>(n + 1));
    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= n; j++) {
            cin >> grid[i][j];

            if (row[grid[i][j]][i] || col[grid[i][j]][j]) {
                cout << "no";
                exit(0);
            }
            row[grid[i][j]][i] = col[grid[i][j]][j] = true;
        }

    for (int i = k + 1; i <= n; i++) {
        vector<pair<int, int>> edges;
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++)
                if (!col[u][v]) edges.emplace_back(u - 1, v - 1);

        auto [l, r, matches] = hopcroft_karp(n, n, edges);
        for (int u = 1; u <= n; u++) {
            int v = l[u - 1] + 1;
            grid[i][v] = u;
            col[u][v] = true;
        }
    }

    cout << "yes\n";
    for (int i = 1; i <= n; i++) {
        cout << grid[i][1];
        for (int j = 2; j <= n; j++) cout << " " << grid[i][j];
        cout << "\n";
    }
}
