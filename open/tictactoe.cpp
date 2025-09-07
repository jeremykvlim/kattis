#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> board(n);
    for (auto &row : board) cin >> row;

    vector<int> XO(2, 0);
    bool filled = true;
    for (auto &row : board)
        for (char cell : row)
            if (cell == 'X') XO[0]++;
            else if (cell == 'O') XO[1]++;
            else filled = false;

    int prev = XO[0] == XO[1] ? 1 : XO[0] == XO[1] + 1 ? 0 : -1;
    if (prev == -1) {
        cout << "ERROR";
        exit(0);
    }

    vector<int> count(2, 0), dr{1, 1, 0, -1}, dc{0, 1, 1, 1};
    vector<vector<int>> win(n, vector<int>(n, 0));
    
    auto symbol = [&](int r, int c) {
        return (r < 0 || r >= n || c < 0 || c >= n) ? '.' : board[r][c];
    };

    for (int p = 0; p < 2; p++)
        for (int k = 0; k < 4; k++)
            for (int r = 0; r < n; r++)
                for (int c = 0; c < n; c++)
                    if (symbol(r - dr[k], c - dc[k]) != "XO"[p]) {
                        int seq = 0;
                        while (symbol(r + seq * dr[k], c + seq * dc[k]) == "XO"[p]) seq++;
                        if (seq >= m) {
                            count[p]++;
                            for (int i = 0; i < seq; i++)
                                if (i < m && m + i >= seq) win[r + i * dr[k]][c + i * dc[k]]++;
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
