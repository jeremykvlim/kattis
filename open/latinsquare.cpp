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

    vector<vector<int>> square(n, vector<int>(n));
    vector<bool> seen(n + 1, false);
    for (auto &row : square)
        for (int &cell : row) {
            cin >> cell;

            seen[cell] = true;
        }

    for (int cell = 1; cell <= n; cell++) {
        if (seen[cell]) continue;

        vector<pair<int, int>> edges;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (!square[i][j]) edges.emplace_back(i, j);

        auto [l, r, matches] = hopcroft_karp(n, n, edges);
        for (int i = 0; i < n; i++) square[r[i]][i] = cell;
    }

    cout << "YES\n";
    for (auto row : square) {
        for (int cell : row) cout << cell << " ";
        cout << "\n";
    }
}
