#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
pair<vector<int>, U> hungarian(const vector<vector<T>> &C, const U delta) {
    int n = C.size();

    vector<int> r_match(n, -1), c_match(n, -1);
    vector<T> potential(n, 0);
    for (int c = 0; c < n; c++) {
        int r = 0;
        for (int row = 1; row < n; row++)
            if (C[r][c] > C[row][c]) r = row;

        potential[c] = C[r][c];
        if (r_match[r] == -1) {
            r_match[r] = c;
            c_match[c] = r;
        }
    }

    auto diff = [&](int r, int c) {
        return C[r][c] - potential[c];
    };

    vector<int> cols(n);
    iota(cols.begin(), cols.end(), 0);
    for (int r = 0; r < n; r++) {
        if (r_match[r] != -1) continue;

        vector<T> dist(n);
        for (int c = 0; c < n; c++) dist[c] = diff(r, c);

        vector<int> prev(n, r);
        int scanned = 0, labeled = 0, last = 0, curr;
        for (;;) {
            if (scanned == labeled) {
                auto d = dist[cols[scanned]];
                for (int c = scanned; c < n; c++)
                    if (dist[cols[c]] <= d) {
                        if (dist[cols[c]] < d) {
                            d = dist[cols[c]];
                            labeled = scanned;
                        }

                        swap(cols[c], cols[labeled++]);
                    }

                for (int c = scanned; c < labeled; c++)
                    if (c_match[cols[c]] == -1) {
                        curr = cols[c];
                        goto done;
                    }

                last = scanned;
            }

            int c1 = cols[scanned++], r1 = c_match[c1];
            for (int c = labeled; c < n; c++) {
                int c2 = cols[c];
                auto d = diff(r1, c2) - diff(r1, c1);

                if (dist[c2] > dist[c1] + d) {
                    dist[c2] = dist[c1] + d;
                    prev[c2] = r1;

                    if (!d) {
                        if (c_match[c2] == -1) {
                            curr = c2;
                            goto done;
                        }

                        swap(cols[c], cols[labeled++]);
                    }
                }
            }
        }

        done:;
        for (int c = 0; c < last; c++) potential[cols[c]] += dist[cols[c]] - dist[curr];
        int temp = curr;
        while (temp != -1) {
            curr = temp;
            c_match[curr] = prev[curr];
            swap(r_match[prev[curr]], temp);
        }
    }

    U cost = 0;
    for (int r = 0; r < n; r++) cost += C[r][r_match[r]];
    return {r_match, cost - delta};
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

    auto [assignment, _] = hungarian(cost, 0);

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
