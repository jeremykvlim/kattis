#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n && m){
        vector<string> grid(m);
        for (auto &row : grid) cin >> row;
        auto temp = grid;

        vector<pair<char, pair<int, int>>> houses;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (isupper(grid[i][j])) houses.emplace_back(grid[i][j], make_pair(i, j));
        sort(houses.begin(), houses.end());

        vector<vector<vector<array<int, 5>>>> dp(1 << 4, vector<vector<array<int, 5>>>(m, vector<array<int, 5>>(n, {(int) 1e9, -2, -1, -1, -1})));
        for (int i = 0; i < 4; i++) {
            auto [r, c] = houses[i].second;
            dp[1 << i][r][c][0] = 0;
            dp[1 << i][r][c][1] = -1;
        }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        for (int m1 = 1; m1 < 1 << 4; m1++) {
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    for (int m2 = (m1 - 1) & m1; m2; --m2 &= m1) {
                        int d = dp[m2][i][j][0] + dp[m1 ^ m2][i][j][0] - (grid[i][j] == 'o');
                        if (dp[m1][i][j][0] > d) {
                            dp[m1][i][j][0] = d;
                            dp[m1][i][j][1] = 0;
                            dp[m1][i][j][4] = m2;
                        }
                    }

            deque<pair<int, int>> dq;
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    if (dp[m1][i][j][0] != 1e9) dq.emplace_back(i, j);

            while (!dq.empty()) {
                auto [i, j] = dq.front();
                dq.pop_front();

                int w = dp[m1][i][j][0];
                for (int k = 0; k < 4; k++){
                    int x = i + dx[k], y = j + dy[k];
                    if (!(0 <= x && x < m && 0 <= y && y < n) || grid[x][y] == '#') continue;
                    int d = w + (grid[x][y] == 'o');
                    if (dp[m1][x][y][0] > d) {
                        dp[m1][x][y][0] = d;
                        dp[m1][x][y][1] = 1;
                        dp[m1][x][y][2] = i;
                        dp[m1][x][y][3] = j;
                        if (grid[x][y] == 'o') dq.emplace_back(x, y);
                        else dq.emplace_front(x, y);
                    }
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
            vector<vector<vector<bool>>> used(1 << 4, vector<vector<bool>>(m, vector<bool>(n, false)));
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            auto dfs = [&](auto &&self, int mask, int i, int j) {
                if (used[mask][i][j]) return;
                used[mask][i][j] = visited[i][j] = true;

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

        int count = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (temp[i][j] == 'o' && grid[i][j] == '.') count++;
        cout << count << "\n";
    }
}
