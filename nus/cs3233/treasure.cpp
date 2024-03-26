#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<char>> grid(n, vector<char>(m));
    pair<int, int> s;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < m; c++) {
            cin >> grid[r][c];

            if (grid[r][c] == 'S') s = {r, c};
        }

    vector<int> dr{0, 0, 1, -1}, dc{1, -1, 0, 0};
    unordered_map<char, int> cost{{'.', 1}, {'F', 2}, {'M', 3}, {'G', 1}};
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(m, vector<int>(k + 1, INT_MAX)));
    dist[s.first][s.second][k] = 1;
    priority_queue<tuple<int, pair<int, int>, int>, vector<tuple<int, pair<int, int>, int>>, greater<>> pq;
    pq.emplace(1, s, k);
    while (!pq.empty()) {
        auto [d, p, stam] = pq.top();
        pq.pop();
        auto [x, y] = p;

        if (d > dist[x][y][stam]) continue;

        if (grid[x][y] == 'G') {
            cout << d;
            exit(0);
        }

        for (int i = 0; i < 4; i++) {
            int r = x + dr[i], c = y + dc[i];
            if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] == '#' || grid[r][c] == 'S' || cost[grid[r][c]] > stam) continue;

            if (dist[r][c][stam - cost[grid[r][c]]] > dist[x][y][stam]) {
                dist[r][c][stam - cost[grid[r][c]]] = dist[x][y][stam];
                pq.emplace(dist[r][c][stam - cost[grid[r][c]]], make_pair(r, c), stam - cost[grid[r][c]]);
            }
        }

        if (dist[x][y][k] > dist[x][y][stam] + 1) {
            dist[x][y][k] = dist[x][y][stam] + 1;
            pq.emplace(dist[x][y][k], p, k);
        }
    }

    cout << -1;
}
