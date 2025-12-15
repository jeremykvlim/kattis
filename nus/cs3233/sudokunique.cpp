#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> board(9, vector<int>(9));
    for (;;) {
        for (auto &row : board)
            for (int &square : row) {
                char c;
                if (!(cin >> c)) exit(0);

                square = c - '0';
            }

        bool valid = true;
        int erased = 0;
        vector<int> row_masks(9, 0), col_masks(9, 0), board_masks(9, 0);
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                int d = board[r][c];
                if (!d) {
                    erased++;
                    continue;
                }

                int b = 1 << (d - 1), square = (r / 3) * 3 + (c / 3);
                if ((row_masks[r] & b) || (col_masks[c] & b) || (board_masks[square] & b)) {
                    valid = false;
                    break;
                }
                row_masks[r] |= b;
                col_masks[c] |= b;
                board_masks[square] |= b;
            }
            if (!valid) break;
        }

        if (!valid) {
            cout << "Find another job\n\n";
            continue;
        }

        bool unique = true;
        int all = (1 << 9) - 1;
        vector<vector<int>> solution;
        auto dfs = [&](auto &&self) -> void {
            if (!unique) return;
            if (!erased) {
                if (!solution.empty()) unique = false;
                else solution = board;
                return;
            }

            int count = 10, row = -1, col = -1, mask = -1;
            for (int r = 0; r < 9; r++)
                for (int c = 0; c < 9; c++)
                    if (!board[r][c]) {
                        int m = all & ~(row_masks[r] | col_masks[c] | board_masks[(r / 3) * 3 + (c / 3)]), pc = popcount((unsigned) m);
                        if (!pc) return;
                        if (count > pc) {
                            count = pc;
                            row = r;
                            col = c;
                            mask = m;
                        }
                    }
            int square = (row / 3) * 3 + (col / 3);
            
            for (int m = mask; m;) {
                int lsb = m & -m, v = countr_zero((unsigned) lsb) + 1;

                board[row][col] = v;
                row_masks[row] ^= lsb;
                col_masks[col] ^= lsb;
                board_masks[square] ^= lsb;
                erased--;
                self(self);
                erased++;
                board_masks[square] ^= lsb;
                col_masks[col] ^= lsb;
                row_masks[row] ^= lsb;
                board[row][col] = 0;

                if (!unique) return;
                m -= lsb;
            }
        };
        dfs(dfs);

        if (solution.empty()) {
            cout << "Find another job\n\n";
            continue;
        }

        if (!unique) {
            cout << "Non-unique\n\n";
            continue;
        }

        for (auto &row : solution) {
            for (int square : row) cout << (char) ('0' + square) << " ";
            cout << "\n";
        }
        cout << "\n";
    }
}
