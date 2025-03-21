#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    if (n == 1 || m == 1) {
        vector<int> board(max(n, m));
        for (int &c : board) cin >> c;

        board[0] = max(board[0], 1);
        for (int i = 1; i < max(n, m); i++) {
            if (board[i - 1] >= board[i] && board[i]) {
                cout << -1;
                exit(0);
            }
            board[i] = max(board[i], board[i - 1] + 1);
        }
        cout << accumulate(board.begin(), board.end(), 0);
        exit(0);
    }

    vector<vector<int>> board(n, vector<int>(m));
    for (auto &row : board)
        for (int &c : row) cin >> c;

    auto sum = LLONG_MAX;
    auto calc = [&](bool x, bool y) -> void {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                if (!board[i][j]) continue;
                if ((i & 1) != (j & 1) && (board[i][j] & 1) != (x ^ !(i & 1)) ||
                   ((i & 1) == (j & 1) && (board[i][j] & 1) != (y ^ !(i & 1)))) return;
            }

        auto s = 0LL;
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]) + 1;
                if ((i & 1) != (j & 1) && (dp[i + 1][j + 1] & 1) != (x ^ !(i & 1))) dp[i + 1][j + 1]++;
                if ((i & 1) == (j & 1) && (dp[i + 1][j + 1] & 1) != (y ^ !(i & 1))) dp[i + 1][j + 1]++;
                if (board[i][j] && dp[i + 1][j + 1] > board[i][j]) return;
                dp[i + 1][j + 1] = max(dp[i + 1][j + 1], board[i][j]);
                s += dp[i + 1][j + 1];
            }
        sum = min(sum, s);
    };
    calc(false, false);
    calc(false, true);
    calc(true, false);
    calc(true, true);
    cout << (sum == LLONG_MAX ? -1 : sum);
}
