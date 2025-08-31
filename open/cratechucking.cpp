#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;

    vector<string> grid(h);
    int sr = -1, sc = -1, er = -1, ec = -1;
    for (int i = 0; i < h; i++) {
        cin >> grid[i];

        for (int j = 0; j < w; j++) {
            if (grid[i][j] == 'S') {
                grid[i][j] = '.';
                sr = i;
                sc = j;
            }

            if (grid[i][j] == 'E') {
                grid[i][j] = '.';
                er = i;
                ec = j;
            }
        }
    }

    auto valid = [&](int r, int c) {
        return 0 <= r && r < h && 0 <= c && c < w && grid[r][c] != 'B';
    };

    auto index = [&](int r, int c, int state, int dir) {
        return (((r * w) + c) * 3 + state) * 5 + dir;
    };

    vector<int> dist(h * w * 3 * 5, 1e9);
    dist[index(sr, sc, 0, 4)] = 0;
    priority_queue<array<int, 5>, vector<array<int, 5>>, greater<>> pq;
    pq.push({0, sr, sc, 0, 4});
    while (!pq.empty()) {
        auto [d, r, c, curr_state, curr_dir] = pq.top();
        pq.pop();

        if (d != dist[index(r, c, curr_state, curr_dir)]) continue;

        if (!curr_state && r == er && c == ec) {
            cout << d;
            exit(0);
        }

        for (int dir = 0; dir < 4; dir++) {
            if (dir == curr_dir) continue;

            int x = -1, y = -1, state = -1, e = -1;
            if (!curr_state) {
                if (!dir && valid(r - 2, c) && valid(r - 1, c)) {
                    x = r - 2;
                    y = c;
                    state = 2;
                    e = 1;
                } else if (dir == 1 && valid(r, c + 1) && valid(r, c + 2)) {
                    x = r;
                    y = c + 1;
                    state = 1;
                    e = 1;
                } else if (dir == 2 && valid(r + 1, c) && valid(r + 2, c)) {
                    x = r + 1;
                    y = c;
                    state = 2;
                    e = 1;
                } else if (dir == 3 && valid(r, c - 2) && valid(r, c - 1)) {
                    x = r;
                    y = c - 2;
                    state = 1;
                    e = 1;
                }
            } else if (curr_state == 1) {
                if (!dir && valid(r - 1, c) && valid(r - 1, c + 1)) {
                    x = r - 1;
                    y = c;
                    state = 1;
                    e = 2;
                } else if (dir == 1 && valid(r, c + 2)) {
                    x = r;
                    y = c + 2;
                    state = 0;
                    e = 3;
                } else if (dir == 2 && valid(r + 1, c) && valid(r + 1, c + 1)) {
                    x = r + 1;
                    y = c;
                    state = 1;
                    e = 2;
                } else if (dir == 3 && valid(r, c - 1)) {
                    x = r;
                    y = c - 1;
                    state = 0;
                    e = 3;
                }
            } else {
                if (!dir && valid(r - 1, c)) {
                    x = r - 1;
                    y = c;
                    state = 0;
                    e = 3;
                } else if (dir == 1 && valid(r, c + 1) && valid(r + 1, c + 1)) {
                    x = r;
                    y = c + 1;
                    state = 2;
                    e = 2;
                } else if (dir == 2 && valid(r + 2, c)) {
                    x = r + 2;
                    y = c;
                    state = 0;
                    e = 3;
                } else if (dir == 3 && valid(r, c - 1) && valid(r + 1, c - 1)) {
                    x = r;
                    y = c - 1;
                    state = 2;
                    e = 2;
                }
            }

            if (!~x && !~y && !~state && !~e) continue;
            if (dist[index(x, y, state, dir)] > d + e) {
                dist[index(x, y, state, dir)] = d + e;
                pq.push({d + e, x, y, state, dir});
            }
        }
    }
    cout << "impossible";
}
