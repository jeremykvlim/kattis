#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n && m) {
        vector<string> grid(m);
        for (auto &row : grid) cin >> row;

        vector<vector<vector<array<int, 5>>>> dp(1 << 4, vector<vector<array<int, 5>>>(m, vector<array<int, 5>>(n, {(int) 1e9, -1, -1, -1, -1})));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (isupper(grid[i][j])) {
                    int mask = 1 << (grid[i][j] - 'A');
                    dp[mask][i][j][0] = 0;
                    dp[mask][i][j][2] = i;
                    dp[mask][i][j][3] = j;
                }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        bool updated = true;
        while (updated) {
            updated = false;
            for (int m1 = 1; m1 < 1 << 4; m1++) {
                if (__builtin_popcount(m1) < 2) continue;
                for (int i = 0; i < m; i++)
                    for (int j = 0; j < n; j++)
                        for (int m2 = (m1 - 1) & m1; m2; --m2 &= m1)
                            if (dp[m2][i][j][0] != 1e9 && dp[m1 ^ m2][i][j][0] != 1e9) {
                                int d = dp[m2][i][j][0] + dp[m1 ^ m2][i][j][0] - (grid[i][j] == 'o');
                                if (dp[m1][i][j][0] > d) {
                                    dp[m1][i][j][0] = d;
                                    dp[m1][i][j][1] = 0;
                                    dp[m1][i][j][2] = i;
                                    dp[m1][i][j][3] = j;
                                    dp[m1][i][j][4] = m2;
                                    updated = true;
                                }
                            }

            }

            for (int mask = 1; mask < 1 << 4; mask++)
                for (int i = 0; i < m; i++)
                    for (int j = 0; j < n; j++)
                        if (dp[mask][i][j][0] != 1e9)
                            for (int k = 0; k < 4; k++) {
                                int x = i + dx[k], y = j + dy[k];
                                if (!(0 <= x && x < m && 0 <= y && y < n) || grid[x][y] == '#') continue;
                                int d = dp[mask][i][j][0] + (grid[x][y] == 'o');
                                if (dp[mask][x][y][0] > d) {
                                    dp[mask][x][y][0] = d;
                                    dp[mask][x][y][1] = 1;
                                    dp[mask][x][y][2] = i;
                                    dp[mask][x][y][3] = j;
                                    updated = true;
                                }
                            }
        }

        int d = 1e9, r = -1, c = -1;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (d > dp.back()[i][j][0]) {
                    d = dp.back()[i][j][0];
                    r = i;
                    c = j;
                }

        if (~r && ~c) {
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            auto dfs = [&](auto &&self, int mask, int i, int j) ->void {
                visited[i][j] = true;

                if (!dp[mask][i][j][1]) {
                    self(self, dp[mask][i][j][4], i, j);
                    self(self, mask ^ dp[mask][i][j][4], i, j);
                } else if (dp[mask][i][j][1] == 1) self(self, mask, dp[mask][i][j][2], dp[mask][i][j][3]);
            };
            dfs(dfs, (1 << 4) - 1, r, c);

            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    if (visited[i][j] && grid[i][j] == 'o') grid[i][j] = '.';
        }

        cout << n << " " << m << "\n";
        for (auto row : grid) cout << row << "\n";
        cout << "\n";
    }
    cout << "0 0";
}
