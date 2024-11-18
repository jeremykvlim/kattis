#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<vector<int>> grid(r, vector<int>(c));
    int max_d = 0, x = -1, y = -1;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            char ch;
            cin >> ch;

            if (ch == 'A') grid[i][j] = -2;
            else if (ch == '.') grid[i][j] = -1;
            else if (isdigit(ch)) grid[i][j] = ch - '0';
            else grid[i][j] = 10 + (int) (ch - 'a');

            max_d = max(max_d, grid[i][j]);
            if (!grid[i][j]) {
                x = i;
                y = j;
            }
        }
    
    if (!max_d) {
        cout << 1;
        exit(0);
    }

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    int j = -1;
    for (int i = 0; i < 4; i++) {
        int X = x - dx[i], Y = y - dy[i];
        if (!(X < 0 || X > r - 1 || Y < 0 || Y > c - 1) && grid[X][Y] == 1) j = i;
    }

    vector<vector<bool>> visited(r, vector<bool>(c, false));
    auto dfs = [&](auto &&self, int x, int y, int j, int depth = 0) -> void {
        visited[x][y] = true;
        if (depth == max_d && r != 1 && c != 1) {
            cout << 1;
            exit(0);
        }

        for (int i = 0; i < 4; i++) {
            int X = x + dx[i], Y = y + dy[i];
            if ((X < 0 || X > r - 1 || Y < 0 || Y > c - 1) || visited[X][Y] || abs(i - j) == 2) continue;

            if (grid[X][Y] == -2 || max_d - depth <= grid[X][Y] && grid[X][Y] != -1) {
                cout << 1;
                exit(0);
            } else if (grid[X][Y] == -1) self(self, X, Y, i, depth + 1);
        }
        visited[x][y] = false;
    };
    dfs(dfs, x, y, j);
    cout << 0;
}
