#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    int nm = n * m;

    vector<string> board(n);
    for (int i = 0; i < n; i++) cin >> board[i];

    if (n > m) {
        vector<string> temp(m, string(n, '#'));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                temp[j][i] = board[i][j];
        swap(n, m);
        board = temp;
    }

    vector<long long> adj_masks(nm, 0);
    for (int i = 0; i < nm; i++) {
        int ri = i / m, ci = i % m;
        for (int j = i + 1; j < nm; j++) {
            int rj = j / m, cj = j % m;
            if (ri == rj || ci == cj || ri + ci == rj + cj || ri - ci == rj - cj) {
                adj_masks[i] |= 1LL << j;
                adj_masks[j] |= 1LL << i;
            }
        }
    }

    auto cell = [&](int r, int c) { return r * m + c; };
    vector<vector<long long>> row_masks(n, {0});
    for (int r = 0; r < n; r++)
        for (int c1 = 0; c1 < m; c1++)
            if (board[r][c1] == '.') {
                row_masks[r].emplace_back(1LL << cell(r, c1));
                for (int c2 = c1 + 1; c2 < m; c2++)
                    if (board[r][c2] == '.')
                        row_masks[r].emplace_back((1LL << cell(r, c1)) | (1LL << cell(r, c2)));
            }

    vector<int> empty(n + 1, 0);
    for (int r = n - 1; ~r; r--) {
        int count = 0;
        for (int c = 0; c < m; c++)
            if (board[r][c] == '.') count++;
        empty[r] = empty[r + 1] + min(2, count);
    }

    int queens = 0;
    auto ways = 0LL, curr = 0LL;
    auto dfs = [&](auto &&self, int r = 0, int placed = 0) {
        if (r == n) {
            if (queens < placed) {
                queens = placed;
                ways = 1;
            } else if (placed == queens) ways++;
            return;
        }

        if (placed + empty[r] < queens) return;

        for (auto m1 : row_masks[r]) {
            int p = popcount((unsigned long long) m1);
            if (placed + p + empty[r + 1] < queens) continue;

            vector<int> cells;
            while (m1) {
                int i = countr_zero((unsigned long long) m1);

                auto m2 = curr & adj_masks[i];
                if (popcount((unsigned long long) m2) >= 2)
                    for (auto t = m2; t; t &= t - 1) {
                        int j = countr_zero((unsigned long long) t);
                        if (m2 & adj_masks[j]) goto next;
                    }
                curr |= 1LL << i;
                cells.emplace_back(i);
                m1 &= m1 - 1;
            }
            self(self, r + 1, placed + p);

            next:;
            for (int i : cells) curr &= ~(1LL << i);
        }
    };
    dfs(dfs);
    cout << queens << " " << ways;
}
