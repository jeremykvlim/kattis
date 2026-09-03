#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, atk1, ar1, atk2, ar2;
    cin >> n >> atk1 >> ar1 >> atk2 >> ar2;

    int m = n + 2;
    vector<vector<int>> grid(m, vector<int>(m, 0)), hp(m, vector<int>(m, 0));
    for (int r = 1; r <= n; r++) {
        string row;
        cin >> row;

        for (int c = 1; c <= n; c++)
            if (row[c - 1] != '.') {
                grid[r][c] = row[c - 1] - '0';
                hp[r][c] = 35;
            }
    }

    int t;
    cin >> t;

    vector<int> damage{0, 5 + atk1 - ar2, 5 + atk2 - ar1}, dr{-1, -1, 0, 1, 1, 1, 0, -1}, dc{0, 1, 1, 1, 0, -1, -1, -1};
    while (t--) {
        int alive = 0;
        vector<vector<vector<int>>> dist(2, vector<vector<int>>(m, vector<int>(m, 2 * n + 1)));
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= n; c++)
                if (grid[r][c]) {
                    alive |= 1 << grid[r][c];
                    dist[grid[r][c] - 1][r][c] = 0;
                }
        if (alive != 6) break;

        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= n; c++)
                for (auto &d : dist) d[r][c] = min(d[r][c], min(d[r - 1][c], d[r][c - 1]) + 1);

        for (int r = n; r; r--)
            for (int c = n; c; c--)
                for (auto &d : dist) d[r][c] = min(d[r][c], min(d[r + 1][c], d[r][c + 1]) + 1);

        vector<vector<int>> dir(m, vector<int>(m, -1));
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= n; c++)
                if (grid[r][c]) {
                    int k = 0;
                    for (int j = 1; j < 8; j++)
                        if (dist[2 - grid[r][c]][r + dr[k]][c + dc[k]] > dist[2 - grid[r][c]][r + dr[j]][c + dc[j]]) k = j;

                    int x = r + dr[k], y = c + dc[k];
                    if (!dist[2 - grid[r][c]][x][y]) hp[x][y] -= damage[grid[r][c]];
                    else dir[r][c] = k;
                }

        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= n; c++)
                if (grid[r][c] && hp[r][c] <= 0) grid[r][c] = 0;

        auto valid = [&](int x, int y, int r, int c) {
            int k = dir[x][y];
            return ~k && x + dr[k] == r && y + dc[k] == c;
        };

        queue<pair<int, int>> q;
        vector<vector<bool>> visited(m, vector<bool>(m, false));
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= n; c++) {
                if (grid[r][c] && !~dir[r][c] && !visited[r][c]) {
                    visited[r][c] = true;
                    q.emplace(r, c);
                }

                int count = 0;
                pair<int, int> winner{m, m};
                for (int k = 0; k < 8; k++) {
                    int x = r + dr[k], y = c + dc[k];
                    if (valid(x, y, r, c)) {
                        count++;
                        winner = min(winner, {x, y});
                    }
                }

                if (count > 1)
                    for (int k = 0; k < 8; k++) {
                        int x = r + dr[k], y = c + dc[k];
                        if (valid(x, y, r, c) && winner != make_pair(x, y) && !visited[x][y]) {
                            visited[x][y] = true;
                            q.emplace(x, y);
                        }
                    }
            }

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int k = 0; k < 8; k++) {
                int x = r + dr[k], y = c + dc[k];
                if (valid(x, y, r, c) && !visited[x][y]) {
                    visited[x][y] = true;
                    q.emplace(x, y);
                }
            }
        }

        vector<vector<int>> temp_grid(m, vector<int>(m, 0)), temp_hp(m, vector<int>(m, 0));
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= n; c++)
                if (grid[r][c]) {
                    int k = dir[r][c], x = visited[r][c] ? r : r + dr[k], y = visited[r][c] ? c : c + dc[k];
                    temp_grid[x][y] = grid[r][c];
                    temp_hp[x][y] = min(35, hp[r][c] + 1);
                }

        grid = temp_grid;
        hp = temp_hp;
    }

    for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= n; c++) cout << (!grid[r][c] ? "." : to_string(grid[r][c]));
        cout << "\n";
    }
}
