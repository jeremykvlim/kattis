#include <bits/stdc++.h>
using namespace std;

char symbol(int r, int c, vector<vector<char>> &board, int n) {
    return (r < 0 || r >= n || c < 0 || c >= n) ? '.' : board[r][c];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<char>> board(n, vector<char>(n));
    for (auto &row : board)
        for (char &cell : row) cin >> cell;

    vector<int> XO(2, 0);
    bool filled = true;
    for (auto &row : board)
        for (char cell : row) {
            if (cell == 'X') XO[0]++;
            else if (cell == 'O') XO[1]++;
            else filled = false;
        }

    int prev = XO[0] == XO[1] ? 1 : XO[0] == XO[1] + 1 ? 0 : -1;
    if (prev == -1) {
        cout << "ERROR";
        exit(0);
    }

    vector<int> count(2, 0), dr = {1, 1, 0, -1}, dc = {0, 1, 1, 1};
    vector<vector<int>> win(n, vector<int>(n, 0));

    for (int p = 0; p < 2; p++)
        for (int k = 0; k < 4; k++)
            for (int row = 0; row < n; row++)
                for (int col = 0; col < n; col++) {
                    if (symbol(row - dr[k], col - dc[k], board, n) != "XO"[p]) {
                        int sequence = 0;
                        while (symbol(row + sequence * dr[k], col + sequence * dc[k], board, n) == "XO"[p]) sequence++;
                        if (sequence >= m) {
                            count[p]++;
                            for (int i = 0; i < sequence; i++)
                                if (i < m && m + i >= sequence) win[row + i * dr[k]][col + i * dc[k]]++;
                        }
                    }
                }

    if (count[1 - prev]) {
        cout << "ERROR";
        exit(0);
    }

    if (!count[prev]) {
        cout << (filled ? "DRAW" : "IN PROGRESS");
        exit(0);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (win[i][j] == count[prev]) {
                cout << "XO"[prev] << " WINS";
                exit(0);
            }

    cout << "ERROR";
}
