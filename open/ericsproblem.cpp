#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    int m;
    cin >> m;

    vector<vector<int>> hori(n, vector<int>(n - 1, 0)), verti(n - 1, vector<int>(n, 0));
    while (m--) {
        int r, c;
        char d;
        cin >> r >> c >> d;
        r--;
        c--;

        if (d == 'L') hori[r][c - 1] = -1;
        if (d == 'R') hori[r][c] = 1;
        if (d == 'U') verti[r - 1][c] = -1;
        if (d == 'D') verti[r][c] = 1;
    }

    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    vector<vector<vector<int>>> perms(n);
    for (int r = 0; r < n; r++) {
        do {
            for (int c = 0; c < n; c++)
                if ((grid[r][c] != '.' && p[c] != grid[r][c] - '0') || (c && hori[r][c - 1] && (p[c - 1] < p[c] ? 1 : -1) != hori[r][c - 1])) goto next;
            perms[r].emplace_back(p);
            next:;
        } while (next_permutation(p.begin(), p.end()));
    }

    int ways = 0;
    vector<int> mask(n, 0);
    auto dfs = [&](auto &&self, const vector<int> &prev, int r = 0) -> void {
        if (r == n) {
            ways++;
            return;
        }

        for (const auto &p : perms[r]) {
            for (int c = 0; c < n; c++)
                if ((mask[c] >> p[c]) & 1 || (r && verti[r - 1][c] && (prev[c] < p[c] ? 1 : -1) != verti[r - 1][c])) goto next;

            for (int c = 0; c < n; c++) mask[c] |= 1 << p[c];
            self(self, p, r + 1);
            for (int c = 0; c < n; c++) mask[c] ^= 1 << p[c];
            next:;
        }
    };
    dfs(dfs, p);
    cout << ways;
}
