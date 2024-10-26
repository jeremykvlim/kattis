#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int h, w;
        cin >> h >> w;

        vector<vector<char>> building(h + 2, vector<char>(w + 2));
        pair<int, int> p1{-1, -1}, p2{-1, -1};
        for (int i = 1; i <= h; i++)
            for (int j = 1; j <= w; j++) {
                cin >> building[i][j];

                if (building[i][j] == '$') {
                    if (p1.first == -1) p1 = {i, j};
                    else p2 = {i, j};
                }
            }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        auto bfs = [&](vector<vector<int>> &dist, pair<int, int> src = {0, 0}) -> void {
            dist[src.first][src.second] = 0;
            deque<pair<int, int>> dq{src};
            while (!dq.empty()) {
                auto [i, j] = dq.front();
                dq.pop_front();

                for (int k = 0; k < 4; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    if (!(0 <= x && x < h + 2 && 0 <= y && y < w + 2) || building[x][y] == '*' || dist[x][y] != INT_MAX) continue;

                    if (building[x][y] == '#') {
                        dist[x][y] = dist[i][j] + 1;
                        dq.emplace_back(x, y);
                    } else {
                        dist[x][y] = dist[i][j];
                        dq.emplace_front(x, y);
                    }
                }
            }
        };

        vector<vector<int>> john(h + 2, vector<int>(w + 2, INT_MAX)),
                            d1(h + 2, vector<int>(w + 2, INT_MAX)),
                            d2(h + 2, vector<int>(w + 2, INT_MAX));
        bfs(john);
        bfs(d1, p1);
        bfs(d2, p2);

        int doors = john[p1.first][p1.second] + john[p2.first][p2.second];
        for (int i = 1; i <= h; i++)
            for (int j = 1; j <= w; j++) {
                int curr = john[i][j] + d1[i][j] + d2[i][j];
                if (building[i][j] == '#') curr -= 2;
                doors = min(doors, curr);
            }

        cout << doors << "\n";
    }
}
