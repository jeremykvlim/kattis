#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for (int X = 1; X <= T; X++) {
        int n, m;
        cin >> n >> m;

        vector<string> grid(n);
        for (auto &s : grid) cin >> s;

        vector<vector<bool>> visited(n, vector<bool>(m, false));
        queue<pair<int, int>> q;
        int t = -1;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                if (grid[i][j] == 'T') {
                    visited[i][j] = true;
                    q.emplace(i, j);
                    t++;
                }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        vector<vector<int>> dist(n, vector<int>(m, 0));
        auto bfs = [&]() {
            while (!q.empty()) {
                auto [i, j] = q.front();
                q.pop();

                for (int k = 0; k < 4; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    if (!(0 <= x && x < n && 0 <= y && y < m) || visited[x][y] || grid[x][y] == '.') continue;
                    dist[x][y] = dist[i][j] + 1;
                    visited[x][y] = true;
                    q.emplace(x, y);
                }
            }
        };
        bfs();

        int Y = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                if (grid[i][j] != '.') Y += dist[i][j];

        grid[0][0] = '#';
        vector<pair<int, int>> path{{0, 0}};
        while (t--) {
            for (auto &row : visited) fill(row.begin(), row.end(), false);
            for (auto [x, y] : path) {
                dist[x][y] = 0;
                visited[x][y] = true;
                q.emplace(x, y);
            }

            bfs();

            int d = INT_MAX, x = -1, y = -1;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    if (d > dist[i][j] && grid[i][j] == 'T') {
                        d = dist[i][j];
                        x = i;
                        y = j;
                    }
            if (d == INT_MAX) break;

            Y += d * (d + 1) / 2 - (d - 1) / 2 * ((d - 1) / 2 + 1);
            if (!(d & 1)) Y -= d / 2;
            grid[x][y] = '#';
            path.emplace_back(x, y);
        }

        cout << "Case #" << X << ": " << Y << "\n";
    }
}
