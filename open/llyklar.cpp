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

    int R, C;
    cin >> R >> C;
    cout << R << " " << C << "\n";

    vector<string> shape(R);
    for (auto &row : shape) cin >> row;

    vector<pair<int, int>> o, x;
    vector<vector<int>> o_id(R, vector<int>(C, -1)), x_id(R, vector<int>(C, -1));
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (shape[i][j] == 'o') {
                o.emplace_back(i, j);
                o_id[i][j] = o.size() - 1;
            } else if (shape[i][j] == 'x') {
                x.emplace_back(i, j);
                x_id[i][j] = x.size() - 1;
            }

    int n = o.size() * 2, m = x.size();
    vector<pair<int, int>> edges;
    vector<vector<pair<int, char>>> dirs(n);
    for (int k = 0; k < o.size(); k++) {
        auto [i, j] = o[k];
        int v = 2 * k, h = 2 * k + 1;
        if (i && shape[i - 1][j] == 'x') {
            int u = x_id[i - 1][j];
            edges.emplace_back(v, u);
            dirs[v].emplace_back(u, 'v');
        }
        if (i + 1 < R && shape[i + 1][j] == 'x') {
            int u = x_id[i + 1][j];
            edges.emplace_back(v, u);
            dirs[v].emplace_back(u, '^');
        }
        if (j && shape[i][j - 1] == 'x') {
            int u = x_id[i][j - 1];
            edges.emplace_back(h, u);
            dirs[h].emplace_back(u, '>');
        }
        if (j + 1 < C && shape[i][j + 1] == 'x') {
            int u = x_id[i][j + 1];
            edges.emplace_back(h, u);
            dirs[h].emplace_back(u, '<');
        }
    }
    auto [l, r, matches] = hopcroft_karp(n, m, edges);

    string dir(m, '^');
    for (int v = 0; v < m; v++)
        for (auto [u, d] : dirs[r[v]])
            if (u == v) {
                dir[v] = d;
                break;
            }

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++)
            if (shape[i][j] == 'x') shape[i][j] = dir[x_id[i][j]];

        cout << shape[i] << "\n";
    }
}
