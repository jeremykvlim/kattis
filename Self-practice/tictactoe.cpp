#include <bits/stdc++.h>
using namespace std;

char symbol(int r, int c, vector<vector<char>>& grid, int n) {
    return (r < 0 || r >= n || c < 0 || c >= n) ? '.' : grid[r][c];
}

void check(int player, char s, vector<vector<int>>& winning, vector<int>& nlines, vector<vector<char>>& grid,
           vector<int> dr, vector<int> dc, int n, int m) {
    for (int k = 0; k < 4; k++)
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++)
                if (symbol(r - dr[k], c - dc[k], grid, n) != s) {
                    int sequence = 0;
                    while (symbol(r + sequence * dr[k], c + sequence * dc[k], grid, n) == s) sequence++;
                    if (sequence >= m) {
                        nlines[player]++;
                        for (int i = 0; i < sequence; i++)
                            if (i < m && m + i >= sequence) winning[r + i * dr[k]][c + i * dc[k]]++;
                    }
                }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<char>> board(n, vector<char>(n));
    for (auto &row : board)
        for (char &cell : row) cin >> cell;

    vector<int> symbol(2, 0);
    bool filled = true;
    for (auto &row : board)
        for (char cell : row) {
            if (cell == 'X') symbol[0]++;
            else if (cell == 'O') symbol[1]++;
            else filled = false;
        }

    int previous = (symbol[0] == symbol[1]) ? 1 : (symbol[0] == symbol[1] + 1) ? 0 : -1;
    if (previous == -1) {
        cout << "ERROR";
        exit(0);
    }

    vector<int> count(2, 0), dr = {1, 1, 0, -1}, dc = {0, 1, 1, 1};
    vector<vector<int>> winning(n, vector<int>(n, 0));
    for (int p = 0; p < 2; p++)
        check(p, "XO"[p], winning, count, board, dr, dc, n, m);

    if (count[1 - previous]) {
        cout << "ERROR";
        exit(0);
    }

    if (!count[previous]) {
        cout << (filled ? "DRAW" : "IN PROGRESS");
        exit(0);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (winning[i][j] == count[previous]) {
                cout << "XO"[previous] << " WINS";
                exit(0);
            }

    cout << "ERROR";
}
