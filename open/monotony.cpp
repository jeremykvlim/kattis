#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<vector<int>> grid(r, vector<int>(c));
    for (auto &row : grid)
        for (int &x : row) cin >> x;

    vector<vector<int>> lt_mask(r, vector<int>(r, 0));
    for (int i1 = 0; i1 < r; i1++)
        for (int i2 = 0; i2 < r; i2++)
            for (int j = 0; j < c; j++)
                if (grid[i1][j] < grid[i2][j]) lt_mask[i1][i2] |= 1 << j;

    auto count = 0LL;
    stack<pair<int, int>> undo;
    vector<vector<long long>> dp(r, vector<long long>(1 << c, 0));
    for (auto m1 = 1U; m1 < 1 << c; m1++) {
        auto rows = 0U;
        for (int i = 0; i < r; i++) {
            bool inc = true, dec = true;
            for (auto m2 = m1, prev = 0U; m2; m2 &= m2 - 1) {
                int x = grid[i][countr_zero(m2)];
                if (prev) {
                    if (prev > x) inc = false;
                    if (prev < x) dec = false;
                }
                prev = x;
            }
            if (inc || dec) rows |= 1 << i;
        }

        for (auto m2 = rows; m2; m2 &= m2 - 1) {
            int i2 = countr_zero(m2);
            for (auto m3 = rows & ((1 << i2) - 1); m3; m3 &= m3 - 1) {
                int i1 = countr_zero(m3);

                auto m4 = lt_mask[i1][i2] & m1;
                dp[i2][m4] += dp[i1][m4] + 1;
                count += dp[i1][m4] + 1;
                undo.emplace(i2, m4);
            }
        }

        count += popcount(rows);
        while (!undo.empty()) {
            auto [i, mask] = undo.top();
            undo.pop();

            dp[i][mask] = 0;
        }
    }
    cout << count;
}
