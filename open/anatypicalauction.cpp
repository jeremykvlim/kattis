#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<vector<int>, T> jonker_volgenant(const vector<vector<T>> &C) {
    int n = C.size(), m = C[0].size();

    vector<T> dist(m), potential(m);
    vector<int> row_match(n, -1), col_match(m, -1), cols(m), prev(m);
    iota(cols.begin(), cols.end(), 0);
    T d = 0;
    for (int i = 0, c1 = -1, temp = 0; i < n; i++) {
        for (int c = 0; c < m; c++) {
            dist[c] = C[i][c] - potential[c];
            prev[c] = i;
        }

        int s = 0, t = 0;
        auto dijkstra = [&]() {
            if (s == t) {
                temp = s;
                d = dist[cols[t++]];
                for (int j = t; j < m; j++) {
                    c1 = cols[j];
                    if (d < dist[c1]) continue;
                    if (d > dist[c1]) {
                        d = dist[c1];
                        t = s;
                    }
                    cols[j] = exchange(cols[t++], c1);
                }

                for (int j = s; j < t; j++)
                    if (!~col_match[c1 = cols[j]]) return true;
            }

            int c2 = cols[s++], r = col_match[c2];
            for (int j = t; j < m; j++) {
                c1 = cols[j];
                if (dist[c1] > C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d) {
                    dist[c1] = C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d;
                    prev[c1] = r;

                    if (dist[c1] == d) {
                        if (!~col_match[c1]) return true;
                        cols[j] = exchange(cols[t++], c1);
                    }
                }
            }
            return false;
        };
        while (!dijkstra());

        for (int j = 0; j < temp; j++) potential[cols[j]] += dist[cols[j]] - d;
        for (int r = -1; r != i;) {
            r = col_match[c1] = prev[c1];
            swap(c1, row_match[r]);
        }
    }

    T cost = 0;
    for (int i = 0; i < n; i++) cost += C[i][row_match[i]];
    return {row_match, cost};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int c_max = 0;
    vector<vector<int>> c(n, vector<int>(m));
    for (auto &row : c)
        for (int &cij : row) {
            cin >> cij;

            c_max = max(c_max, cij);
        }

    vector<vector<int>> cost(m, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) cost[i][j] = c_max - c[i][j];

    auto [assignment, _] = jonker_volgenant(cost);

    int income = 0;
    for (int i = 0; i < n; i++) income += c[i][assignment[i]];

    vector<bool> used(m, false);
    for (int i = 0; i < n; i++) {
        int cik = -1, k = -1;
        for (int j = 0; j < m; j++)
            if (!used[j])
                if (cik < c[i][j]) {
                    cik = c[i][j];
                    k = j;
                }
        used[k] = true;
        income -= cik;
    }
    cout << income << "\n";

    vector<int> degree(n, 0);
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                if (c[i][assignment[j]] > c[i][assignment[i]]) {
                    adj_list[j].emplace_back(i);
                    degree[i]++;
                }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (!degree[i]) q.emplace(i);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        cout << v << " ";

        for (int u : adj_list[v])
            if (!--degree[u]) q.emplace(u);
    }
}
