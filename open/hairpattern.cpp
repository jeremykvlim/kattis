#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> curr(n), desired(n);
    for (auto &row : curr) cin >> row;
    for (auto &row : desired) cin >> row;

    vector<int> popcount(1 << max(n, m), 0);
    for (int mask = 1; mask < 1 << max(n, m); mask++) popcount[mask] = popcount[mask & (mask - 1)] + 1;

    vector<int> row_B(n, 0), col_B(m, 0), row_same(n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            if (desired[i][j] == 'B') {
                row_B[i] |= 1 << j;
                col_B[j] |= 1 << i;
            }
            if (curr[i][j] == desired[i][j]) row_same[i] |= 1 << j;
        }

    vector<vector<int>> row_want_b(n, vector<int>(1 << m, 0)), col_want_b(m, vector<int>(1 << n, 0)), row_match(n, vector<int>(1 << m, 0));
    for (int i = 0; i < n; i++)
        for (int mask = 0; mask < 1 << m; mask++) {
            row_want_b[i][mask] = max(popcount[row_B[i] & mask], popcount[mask] - popcount[row_B[i] & mask]);
            row_match[i][mask] = popcount[row_same[i] & mask];
        }

    for (int j = 0; j < m; j++)
        for (int mask = 0; mask < 1 << n; mask++) col_want_b[j][mask] = max(popcount[col_B[j] & mask], popcount[mask] - popcount[col_B[j] & mask]);

    int squares = 0;
    vector<vector<int>> dp(1 << n);
    for (int m1 = 0; m1 < 1 << m; m1++) {
        vector<int> cols;
        for (int j = 0; j < m; j++)
            if (!((m1 >> j) & 1)) cols.emplace_back(j);

        int size = 1 << cols.size();
        vector<int> col_masks(size, 0);
        for (int c = 1; c < size; c++) {
            int lsb = c & -c;
            col_masks[c] = col_masks[c ^ lsb] | (1 << cols[countr_zero((unsigned) lsb)]);
        }

        for (auto &row : dp) row.assign(size, -1e9);
        dp[0][0] = 0;
        for (int r = 0; r < 1 << n; r++)
            for (int c = 0; c < size; c++)
                if (dp[r][c] != -1e9) {
                    int m2 = m1 | col_masks[c];

                    int rows_rem = ~r & ((1 << n) - 1);
                    while (rows_rem) {
                        int lsb = rows_rem & -rows_rem, i = countr_zero((unsigned) lsb);
                        dp[r | (1 << i)][c] = max({dp[r | (1 << i)][c], dp[r][c] + row_want_b[i][m2], dp[r][c] + row_match[i][m1]});
                        rows_rem ^= lsb;
                    }

                    int cols_rem = (size - 1) ^ c;
                    while (cols_rem) {
                        int lsb = cols_rem & -cols_rem, j = cols[countr_zero((unsigned) lsb)];
                        dp[r][c | lsb] = max(dp[r][c | lsb], dp[r][c] + col_want_b[j][r]);
                        cols_rem ^= lsb;
                    }
                }

        squares = max(squares, dp.back().back());
    }
    cout << squares;
}
