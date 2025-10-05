#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> grid(n);
    int sx = -1, sy = -1, tx = -1, ty = -1;
    for (int i = 0; i < n; i++) {
        cin >> grid[i];

        for (int j = 0; j < n; j++)
            if (grid[i][j] == '@') {
                sx = i;
                sy = j;
            } else if (grid[i][j] == '$') {
                tx = i;
                ty = j;
            }
    }

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    vector<vector<vector<int>>> dist(1 << 10, vector<vector<int>>(n, vector<int>(n, -1)));
    dist[0][sx][sy] = 0;
    queue<array<int, 3>> q;
    q.push({0, sx, sy});
    while (!q.empty()) {
        auto [mask, x, y] = q.front();
        q.pop();

        if (x == tx && y == ty) {
            cout << dist[mask][x][y];
            exit(0);
        }

        for (int k = 0; k < 4; k++) {
            int r = x + dx[k], c = y + dy[k];
            if (!(0 <= r && r < n && 0 <= c && c < n) || grid[r][c] == '#') continue;

            auto m = mask;
            if (isupper(grid[r][c]))
                if (!((mask >> (grid[r][c] - 'A')) & 1)) continue;

            if (islower(grid[r][c])) m |= 1 << (grid[r][c] - 'a');

            if (!~dist[m][r][c]) {
                dist[m][r][c] = dist[mask][x][y] + 1;
                q.push({m, r, c});
            }
        }
    }
    cout << "IMPOSSIBLE";
}
