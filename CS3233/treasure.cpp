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

    priority_queue<tuple<int, pair<int, int>, int>, vector<tuple<int, pair<int, int>, int>>, greater<>> pq;
    vector<vector<vector<int>>> dist(n, vector<vector<int>>(m, vector<int>(k + 1, INT_MAX)));
    vector<int> dr{0, 0, 1, -1}, dc{1, -1, 0, 0};
    unordered_map<char, int> cost = {{'.', 1}, {'F', 2}, {'M', 3}, {'G', 1}};
    dist[s.first][s.second][k] = 1;
    pq.push(make_tuple(1, s, k));

    while (!pq.empty()) {
        auto [d, p, stam] = pq.top();
        pq.pop();
        if (d > dist[p.first][p.second][stam]) continue;
        if (grid[p.first][p.second] == 'G') {
            cout << d;
            exit(0);
        }

        for (int i = 0; i < 4; i++) {
            int r = p.first + dr[i], c = p.second + dc[i];
            if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] == '#' || grid[r][c] == 'S' || cost[grid[r][c]] > stam) continue;

            if (dist[r][c][stam - cost[grid[r][c]]] > dist[p.first][p.second][stam]) {
                dist[r][c][stam - cost[grid[r][c]]] = dist[p.first][p.second][stam];
                pq.push(make_tuple(dist[r][c][stam - cost[grid[r][c]]], make_pair(r, c), stam - cost[grid[r][c]]));
            }
        }

        if (dist[p.first][p.second][k] > dist[p.first][p.second][stam] + 1) {
            dist[p.first][p.second][k] = dist[p.first][p.second][stam] + 1;
            pq.push(make_tuple(dist[p.first][p.second][k], p, k));
        }
    }
                   
    cout << -1;
}
