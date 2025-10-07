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
        for (;;) {
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
                    if (!~col_match[c1 = cols[j]]) goto done;
            }

            int c2 = cols[s++], r = col_match[c2];
            for (int j = t; j < m; j++) {
                c1 = cols[j];
                if (dist[c1] > C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d) {
                    dist[c1] = C[r][c1] - C[r][c2] + potential[c2] - potential[c1] + d;
                    prev[c1] = r;

                    if (dist[c1] == d) {
                        if (!~col_match[c1]) goto done;
                        cols[j] = exchange(cols[t++], c1);
                    }
                }
            }
        }
        done:;

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

    int n, m, r;
    cin >> n >> m >> r;

    vector<vector<int>> C(n + m, vector<int>(n + m, 0));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) C[i][j] = 1e6;

    while (r--) {
        int a, b, c;
        cin >> a >> b >> c;

        C[b - 1][a - 1] = -1e3 - c;
    }

    auto [match, cost] = jonker_volgenant(C);
    auto sum = 0LL;
    int matches = 0;
    for (int i = 0; i < m; i++)
        if (0 <= match[i] && match[i] < n)
            if (C[i][match[i]] != 1e6) {
                sum -= 1e3 + C[i][match[i]];
                matches++;
            }
    cout << n - matches << " " << sum;
}
