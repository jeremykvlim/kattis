#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<string> grid(r, string(c, '.'));
    for (int i = (r < 4 ? 0 : 2); i + 1 < r; i += 5) {
        for (int j = (c < 3 ? 1 : 2); j < c; j += 4) grid[i][j] = grid[i + 1][j] = 'X';
        if (c > 4 && c % 4 == 2) grid[i][c - 1] = grid[i + 1][c - 1] = 'X';
    }

    if (r > 5) {
        if (r % 5 == 2) {
            for (int j = (c < 3 ? 1 : 2); j < c; j += 4) grid[r - 3][j] = 'X';
            if (c > 4 && c % 4 == 2) grid[r - 3][c - 1] = 'X';
        } else if (r % 5 == 3) {
            for (int j = (c < 3 ? 1 : 2); j < c; j += 4) grid[r - 2][j] = grid[r - 1][j] = 'X';
            if (c > 4 && c % 4 == 2) grid[r - 2][c - 1] = grid[r - 1][c - 1] = 'X';
        }
    }

    for (auto &row : grid) cout << row << "\n";
}
