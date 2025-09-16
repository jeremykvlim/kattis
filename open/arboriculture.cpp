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

    int n, m;
    cin >> n >> m;

    auto read = [&](auto &forest) {
        for (auto &tree : forest) {
            int b;
            cin >> b;

            tree.resize(b + 1);
            for (int i = 1; i <= b; i++) {
                int a;
                cin >> a;

                tree[a].emplace_back(i);
            }
        }
    };
    vector<vector<vector<int>>> need(n), have(m);
    read(need);
    read(have);

    vector<vector<int>> memo;
    auto dp = [&](const auto &t1, const auto &t2) {
        memo = vector(t2.size(), vector<int>(t1.size(), 0));
        for (int a = t2.size() - 1; a >= 0; a--)
            for (int b = t1.size() - 1; b >= 0; b--) {
                int s1 = t2[a].size(), s2 = t1[b].size();
                if (!s2) {
                    memo[a][b] = s1;
                    continue;
                }

                if (s1 < s2) {
                    memo[a][b] = 1e6;
                    continue;
                }

                bool leaves = true;
                for (int x : t1[b])
                    if (!t1[x].empty()) {
                        leaves = false;
                        break;
                    }

                if (leaves) {
                    vector<int> sizes;
                    for (int y : t2[a]) sizes.emplace_back(t2[y].size());
                    nth_element(sizes.begin(), sizes.begin() + s2, sizes.end());
                    memo[a][b] = accumulate(sizes.begin(), sizes.begin() + s2, 0) + s1 - s2;
                    continue;
                }

                vector<vector<int>> c(s2, vector<int>(s1, 1e6));
                for (int i = 0; i < s2; i++)
                    for (int j = 0; j < s1; j++) c[i][j] = memo[t2[a][j]][t1[b][i]];
                memo[a][b] = jonker_volgenant(c).second + s1 - s2;
            }

        return memo[0][0];
    };

    vector<vector<int>> cost(n, vector<int>(m, 1e6));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) cost[i][j] = dp(need[i], have[j]);
    cout << jonker_volgenant(cost).second;
}
