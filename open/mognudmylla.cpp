#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string letters{".xXoO"};
    array<int, 9> board{};
    array<int, 5> remaining{0, 4, 2, 4, 2};
    auto print = [&] {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) cout << letters[board[3 * r + c]];
            cout << "\n";
        }
        cout << flush;
    };
    remaining[board[4] = 1]--;
    print();

    vector<array<int, 3>> winning{{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                                  {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                                  {0, 4, 8}, {2, 4, 6}};

    auto win = [&](const auto &board, int turn = 0) {
        for (auto &w : winning)
            if (board[w[0]] && (board[w[0]] - 1) / 2 == turn &&
                board[w[1]] && (board[w[1]] - 1) / 2 == turn &&
                board[w[2]] && (board[w[2]] - 1) / 2 == turn) return true;
        return false;
    };

    auto legal_moves = [&](const auto &board, const auto &remaining, int turn = 0) {
        vector<pair<array<int, 9>, array<int, 5>>> moves;
        if (remaining[2 * turn + 1])
            for (int i = 0; i < 9; i++)
                if (!board[i]) {
                    auto b = board;
                    auto r = remaining;
                    b[i] = 2 * turn + 1;
                    r[2 * turn + 1]--;
                    moves.emplace_back(b, r);
                }

        if (remaining[2 * turn + 2])
            for (int i = 0; i < 9; i++)
                if (!board[i] || board[i] & 1) {
                    auto b = board;
                    auto r = remaining;
                    b[i] = 2 * turn + 2;
                    r[2 * turn + 2]--;
                    moves.emplace_back(b, r);
                }

        for (int i = 0; i < 9; i++)
            if (board[i] == 2 * turn + 2)
                for (int j = 0; j < 9; j++)
                    if (board[j] & 1) {
                        auto b = board;
                        b[i] = 0;
                        b[j] = 2 * turn + 2;
                        moves.emplace_back(b, remaining);
                    }
        return moves;
    };

    unordered_map<int, int> dp;
    for (;;) {
        string s;
        cin >> s;

        if (s.back() == '!') exit(0);

        int o = count(board.begin(), board.end(), 3), O = count(board.begin(), board.end(), 4);
        array<int, 9> temp;
        for (int r = 0; r < 3; r++) {
            if (r) cin >> s;
            for (int c = 0; c < 3; c++) temp[3 * r + c] = letters.find(s[c]);
        }
        board = temp;
        remaining[3] -= count(board.begin(), board.end(), 3) > o;
        remaining[4] -= count(board.begin(), board.end(), 4) > O;

        auto dfs = [&](auto &&self, const auto &board, const auto &remaining, int turn = 1) -> int {
            int state = turn;
            for (int letter : board) state = state * 5 + letter;
            for (int count : remaining) state = state * 5 + count;

            auto [it, inserted] = dp.try_emplace(state, -1);
            if (!inserted) return it->second;

            for (const auto &[b, r] : legal_moves(board, remaining, turn))
                if (win(b, turn) || !self(self, b, r, turn ^ 1)) return it->second = 1;
            return it->second = 0;
        };

        for (const auto &[b, r] : legal_moves(board, remaining))
            if (win(b) || !dfs(dfs, b, r)) {
                board = b;
                remaining = r;
                break;
            }
        print();
    }
}
