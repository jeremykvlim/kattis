#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<vector<int>> grid(m, vector<int>(n));
    for (auto &row : grid)
        for (int &square : row) cin >> square;

    int r, c;
    cin >> r >> c;
    r--;
    c--;

    int moves = 0;
    vector<int> rows, cols;
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    while (0 <= r && r < m && 0 <= c && c < n && grid[r][c] >= 0 && !visited[r][c]) {
        visited[r][c] = true;
        rows.emplace_back(r);
        cols.emplace_back(c);
        moves++;

        if (r >= 2 && grid[r - 1][c] == grid[r][c]) r -= 2;
        else if (r < m - 2 && grid[r + 1][c] == grid[r][c]) r += 2;
        else if (c >= 2 && grid[r][c - 1] == grid[r][c]) c -= 2;
        else if (c < n - 2 && grid[r][c + 1] == grid[r][c]) c += 2;
        else break;
    }

    if (grid[r][c] == -1)
        for (int i = moves - 1; ~i; i--) cout << grid[rows[i]][cols[i]] << " ";
    else cout << "impossible";
}
