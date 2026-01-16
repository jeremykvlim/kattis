#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    while (k--) {
        int n;
        cin >> n;

        vector<string> grid(n);
        for (auto &row : grid) cin >> row;

        int m = 2 * n - 1;
        vector<vector<pair<int, int>>> squares(m);
        vector<vector<int>> indices(n, vector<int>(n));
        vector<vector<__int128>> masks(m, vector<__int128>(26, 0));
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++) {
                squares[r + c].emplace_back(r, c);
                masks[r + c][grid[r][c] - 'A'] |= (__int128) 1 << (indices[r][c] = squares[r + c].size() - 1);
            }

        vector<vector<vector<__int128>>> dp(m, vector<vector<__int128>>(m));
        int longest = 0;
        for (int gap = 0; gap < m; gap++)
            for (int dl = 0; dl + gap < m; dl++) {
                int dr = dl + gap;
                dp[dl][dr].resize(squares[dl].size(), 0);

                if (!gap) {
                    for (int i = 0; i < squares[dl].size(); i++) dp[dl][dr][i] = (__int128) 1 << i;
                    continue;
                }

                if (gap == 1) {
                    for (int i = 0; i < squares[dl].size(); i++) {
                        auto [r, c] = squares[dl][i];
                        if (c + 1 < n && grid[r][c] == grid[r][c + 1]) dp[dl][dr][i] |= (__int128) 1 << indices[r][c + 1];
                        if (r + 1 < n && grid[r][c] == grid[r + 1][c]) dp[dl][dr][i] |= (__int128) 1 << indices[r + 1][c];
                        if (dp[dl][dr][i]) longest = 1;
                    }
                    continue;
                }

                for (int i = 0; i < squares[dl].size(); i++) {
                    auto [r, c] = squares[dl][i];
                    __int128 mask = 0;
                    if (c + 1 < n) mask |= dp[dl + 1][dr - 1][indices[r][c + 1]];
                    if (r + 1 < n) mask |= dp[dl + 1][dr - 1][indices[r + 1][c]];
                    dp[dl][dr][i] = (dr < n ? (mask | (mask << 1)) : (mask | (mask >> 1))) & masks[dr][grid[r][c] - 'A'];
                    if (dp[dl][dr][i]) longest = max(longest, gap);
                }
            }

        pair<int, int> first{-1, -1}, last{-1, -1};
        auto cmp = [&](auto p1, auto p2, auto p3, auto p4) {
            return tie(p1.first, p1.second, p2.first, p2.second) > tie(p3.first, p3.second, p4.first, p4.second);
        };
        for (int dl = 0; dl + longest < m; dl++)
            for (int i = 0, dr = dl + longest; i < squares[dl].size(); i++)
                if (dp[dl][dr][i])
                    for (int j = 0; j < squares[dr].size(); j++)
                        if ((dp[dl][dr][i] >> j) & 1)
                            if ((first == make_pair(-1, -1) && last == make_pair(-1, -1)) || cmp(first, last, squares[dl][i], squares[dr][j])) tie(first, last) = tie(squares[dl][i], squares[dr][j]);

        vector<int> dr_l{0, 1}, dc_l{1, 0}, dr_r{0, -1}, dc_r{-1, 0};
        auto dfs = [&](auto &&self, pair<int, int> square_l, pair<int, int> square_r) -> vector<pair<int, int>> {
            int gap = square_r.first + square_r.second - square_l.first - square_l.second;
            if (!gap) return {square_l};
            if (gap == 1) return {square_l, square_r};

            for (int i = 0; i < 2; i++) {
                int r_l = square_l.first + dr_l[i], c_l = square_l.second + dc_l[i];
                if (!(0 <= r_l && r_l < n && 0 <= c_l && c_l < n)) continue;
                for (int j = 0; j < 2; j++) {
                    int r_r = square_r.first + dr_r[j], c_r = square_r.second + dc_r[j];
                    if (!(0 <= r_r && r_r < n && 0 <= c_r && c_r < n) ||
                        grid[r_l][c_l] != grid[r_r][c_r] ||
                        !((dp[r_l + c_l][r_r + c_r][indices[r_l][c_l]] >> indices[r_r][c_r]) & 1)) continue;

                    vector<pair<int, int>> path;
                    path.emplace_back(square_l);
                    for (auto square : self(self, {r_l, c_l}, {r_r, c_r})) path.emplace_back(square);
                    path.emplace_back(square_r);
                    return path;
                }
            }
            return {square_l};

        };
        auto path = dfs(dfs, first, last);
        for (auto [r, c] : path) cout << grid[r][c];
        cout << " " << first.first + 1 << " " << first.second + 1 << " ";
        for (int i = 0; i + 1 < path.size(); i++) cout << (path[i].first == path[i + 1].first ? 'R' : 'D');
        cout << "S\n";
    }
}
