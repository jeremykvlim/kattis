#include <bits/stdc++.h>
using namespace std;

bool valid_digit(vector<vector<char>> &board, int r, int c) {
    vector<bool> seen_r(10, false), seen_c(10, false);
    for (int i = 0; i < 9; i++) {
        if (board[i][c] != '0' && seen_r[board[i][c] - '0']) return false;
        if (board[r][i] != '0' && seen_c[board[r][i] - '0']) return false;

        seen_r[board[i][c] - '0'] = seen_c[board[r][i] - '0'] = true;
    }

    vector<bool> seen_block(10, false);
    int i = r - r % 3, j = c - c % 3;
    for (int k = 0; k < 3; k++)
        for (int l = 0; l < 3; l++)
            if (board[i + k][j + l] != '0' && seen_block[board[i + k][j + l] - '0']) return false;
            else seen_block[board[i + k][j + l] - '0'] = true;

    return true;
}

void backtrack(vector<vector<char>> &board, vector<vector<char>> &solution, bool &unique) {
    auto solved = [&]() -> bool {
        return !any_of(board.begin(), board.end(), [&](auto row) {
            return any_of(row.begin(), row.end(), [&](char c) { return c == '0'; });
        });
    };

    if (solved()) {
        if (!solution.empty()) unique = false;
        else solution = board;

        return;
    }

    int r, c;
    vector<char> possible(10);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            if (board[i][j] != '0') continue;

            vector<char> p;
            for (char d = '1'; d <= '9'; d++) {
                board[i][j] = d;
                if (valid_digit(board, i, j)) p.emplace_back(d);
                board[i][j] = '0';
            }

            if (possible.size() > p.size()) {
                r = i;
                c = j;
                possible = p;
            }
        }

    for (char d : possible) {
        board[r][c] = d;
        backtrack(board, solution, unique);
        if (!unique) return;
        board[r][c] = '0';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (;;) {
        vector<vector<char>> board(9, vector<char>(9));
        for (auto &row : board)
            for (char &sq : row)
                if (!(cin >> sq)) exit(0);

        auto valid = [&]() -> bool {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++) {
                    if (board[i][j] == '0') continue;
                    if (!valid_digit(board, i, j)) return false;
                }

            return true;
        };

        if (!valid()) {
            cout << "Find another job\n\n";
            continue;
        }

        vector<vector<char>> solution;
        bool unique = true;
        backtrack(board, solution, unique);
        if (solution.empty()) {
            cout << "Find another job\n\n";
            continue;
        }

        if (!unique) {
            cout << "Non-unique\n\n";
            continue;
        }

        for (auto &row : solution) {
            for (char sq : row) cout << sq << " ";
            cout << "\n";
        }
        cout << "\n";
    }
}
