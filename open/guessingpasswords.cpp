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

    int n, m;
    cin >> n >> m;

    vector<vector<bool>> game(n, vector<bool>(m));
    int y = 0;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        for (int j = 0; j < m; j++) {
            game[i][j] = (s[j] == 'Y');
            if (!i) y += game[i][j];
        }
    }

    int sigma;
    cin >> sigma;

    if (y && n >= m || sigma < y + (n + 1) * (m - y)) {
        cout << "Bugged!\n";
        exit(0);
    }

    int c = m + y;
    vector<int> y_count(c, 0), extras;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            y_count[j] += game[i][j];

            if (y_count[j] > y) {
                cout << "Bugged!\n";
                exit(0);
            }

            if (i == n - 1 && y_count[j] < y) extras.emplace_back(j);
        }

    if (extras.size() < y) {
        cout << "Bugged!\n";
        exit(0);
    }

    vector<vector<bool>> g(max(c, n + 1), vector<bool>(c));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) g[i][j] = game[i][j];

    for (int i = 0; i < y; i++) {
        g[n][extras[i]] = true;
        y_count[extras[i]]++;
    }

    for (int i = n + 1; i < g.size(); i++) {
        vector<int> cols(c);
        iota(cols.begin(), cols.end(), 0);
        sort(cols.begin(), cols.end(), [&](int j, int k) { return y_count[j] < y_count[k]; });

        int temp = y;
        for (int j : cols)
            if (temp && y_count[j] < y) {
                g[i][j] = true;
                y_count[j]++;
                temp--;
            }
    }

    vector<vector<int>> sol(n + 1, vector<int>(m));
    int gray = y + 1;
    for (int i = 0; i <= n; i++)
        for (int j = 0; j < m; j++)
            if (!g[i][j]) sol[i][j] = gray++;

    for (int Y = 1; Y <= y; Y++) {
        vector<pair<int, int>> edges;
        for (int i = 0; i < c; i++)
            for (int j = 0; j < c; j++)
                if (g[i][j]) edges.emplace_back(i, j);

        auto [l, r, matches] = hopcroft_karp(c, c, edges);
        for (int i = 0; i < c; i++) {
            g[r[i]][i] = false;
            if (i < m && r[i] <= n) sol[r[i]][i] = Y;
        }
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < m; j++) cout << sol[i][j] << " ";
        cout << "\n";
    }
}
