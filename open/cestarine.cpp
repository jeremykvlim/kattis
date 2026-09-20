#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, vector<int>> jonker_volgenant(const vector<vector<T>> &C) {
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
    return {cost, row_match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> entrance(n), exit(n);
    for (int i = 0; i < n; i++) cin >> entrance[i] >> exit[i];
    sort(entrance.begin(), entrance.end());
    sort(exit.begin(), exit.end());

    vector<long long> dp(n + 1, 1e18);
    dp[n] = 0;
    for (int i = n - 1; ~i; i--)
        for (int j = 1; j <= 3 && i + j <= n; j++) {
            vector<vector<long long>> C(j, vector<long long>(j));
            for (int r = 0; r < j; r++)
                for (int c = 0; c < j; c++) C[r][c] = entrance[i + r] == exit[i + c] ? 1e18 : abs(entrance[i + r] - exit[i + c]);

            dp[i] = min(dp[i], dp[i + j] + jonker_volgenant(C).first);
        }
    cout << dp[0];
}