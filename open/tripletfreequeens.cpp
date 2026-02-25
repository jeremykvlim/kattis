#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> board(n);
    for (auto &row : board) cin >> row;

    if (n > m) {
        vector<string> temp(m, string(n, '#'));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                temp[j][i] = board[i][j];
        swap(n, m);
        board = temp;
    }

    int nm = n * m;
    vector<unsigned long long> adj_masks(nm, 0);
    for (int i = 0; i < nm; i++) {
        int ri = i / m, ci = i % m;
        for (int j = i + 1; j < nm; j++) {
            int rj = j / m, cj = j % m;
            if (ri == rj || ci == cj || ri + ci == rj + cj || ri - ci == rj - cj) {
                adj_masks[i] |= 1ULL << j;
                adj_masks[j] |= 1ULL << i;
            }
        }
    }

    vector<vector<unsigned long long>> common(nm, vector<unsigned long long>(nm, 0));
    for (int i = 0; i < nm; i++)
        for (int j = i + 1; j < nm; j++) common[i][j] = common[j][i] = adj_masks[i] & adj_masks[j];

    auto cell = [&](int r, int c) { return r * m + c; };
    vector<vector<unsigned long long>> row_masks(n);
    for (int r = 0; r < n; r++) {
        vector<int> cols;
        for (int c = 0; c < m; c++)
            if (board[r][c] == '.') cols.emplace_back(c);

        int k = cols.size();
        for (int i = 0; i < k; i++)
            for (int j = i + 1; j < k; j++) row_masks[r].emplace_back((1ULL << cell(r, cols[i])) | (1ULL << cell(r, cols[j])));

        for (int i = 0; i < k; i++) row_masks[r].emplace_back(1ULL << cell(r, cols[i]));
        row_masks[r].emplace_back(0);
    }

    vector<int> empty(n + 1, 0);
    for (int r = n - 1; ~r; r--) {
        int count = 0;
        for (int c = 0; c < m; c++) count += board[r][c] == '.';
        empty[r] = empty[r + 1] + min(2, count);
    }

    int queens = 0;
    auto ways = 0LL;
    auto placed_mask = 0ULL, avoid_mask = 0ULL;
    auto dfs = [&](auto &&self, int r = 0, int placed = 0) {
        if (r == n) {
            if (queens < placed) {
                queens = placed;
                ways = 1;
            } else ways += placed == queens;
            return;
        }

        if (placed + empty[r] < queens) return;

        for (auto m1 : row_masks[r]) {
            int p = popcount(m1);
            if (placed + p + empty[r + 1] < queens) continue;

            auto temp1 = placed_mask, temp2 = avoid_mask;
            for (; m1; m1 &= m1 - 1) {
                int i = countr_zero(m1);
                if ((avoid_mask >> i) & 1) goto next;
                for (auto m2 = placed_mask & adj_masks[i]; m2; m2 &= m2 - 1) avoid_mask |= common[i][countr_zero(m2)];
                placed_mask |= 1ULL << i;
            }

            self(self, r + 1, placed + p);

            next:;
            placed_mask = temp1;
            avoid_mask = temp2;
        }
    };
    dfs(dfs);
    cout << queens << " " << ways;
}
