#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<char>> grid(n, vector<char>(m));
    pair<int, int> s, e;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++) {
            cin >> grid[r][c];

            if (grid[r][c] == 'S') s = {r, c};
            if (grid[r][c] == 'G') e = {r, c};
        }

    vector<int> dr{0, 0, 1, -1}, dc{1, -1, 0, 0};
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(m, vector<int>(k + 1, -1)));
    dist[s.first][s.second][k] = 1;
    priority_queue<array<int, 4>, vector<array<int, 4>>, greater<>> pq;
    pq.push({1, s.first, s.second, k});
    while (!pq.empty()) {
        auto [d, x, y, stam] = pq.top();
        pq.pop();

        for (int i = 0; i < 4; i++) {
            int r = x + dr[i], c = y + dc[i];
            if (r < 0 || r >= n || c < 0 || c >= m) continue;

            switch (grid[r][c]) {
                case 'F':
                    if (stam >= 2)
                        if (dist[r][c][stam - 2] == -1) {
                            dist[r][c][stam - 2] = dist[x][y][stam];
                            pq.push({d, r, c, stam - 2});
                        }
                    break;
                case 'M':
                    if (stam >= 3)
                        if (dist[r][c][stam - 3] == -1) {
                            dist[r][c][stam - 3] = dist[x][y][stam];
                            pq.push({d, r, c, stam - 3});
                        }
                    break;
                case '#':
                    if (dist[r][c][stam] == -1) dist[r][c][stam] = INT_MAX;
                    break;
                default:
                    if (stam >= 1)
                        if (dist[r][c][stam - 1] == -1) {
                            dist[r][c][stam - 1] = dist[x][y][stam];
                            pq.push({d, r, c, stam - 1});
                        }
                    break;
            }
        }

        if (dist[x][y][k] == -1) {
            dist[x][y][k] = d + 1;
            pq.push({d + 1, x, y, k});
        }
    }

    int days = INT_MAX;
    for (int d : dist[e.first][e.second])
        if (d != -1) days = min(days, d);

    cout << (days == INT_MAX ? -1 : days);
}
