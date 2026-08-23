#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, T, R, C;
    cin >> k >> T >> R >> C;

    vector<string> grid(R);
    for (auto &row : grid) cin >> row;

    vector<int> time(R), snow;
    vector<string> after(R, string(C, '.'));
    for (int c = 0; c < C; c++) {
        snow.clear();
        for (int r = 0, prev = -1; r < R; r++)
            if (grid[r][c] == 'S') snow.emplace_back(r);
            else if (grid[r][c] == '#') {
                int w = snow.size();
                time[r] = 1e9;
                if (w >= k) time[r] = r - snow[w - k] - k;
                else if (~prev) time[r] = time[prev] + r - prev - w;

                prev = r;
                snow.clear();
                if (time[r] >= T) after[r][c] = '#';
            }

        snow.clear();
        for (int r = R - 1, prev = R, t = 1e9, barrier = R; ~r; r--)
            if (grid[r][c] == '#') {
                barrier = r;
                t = time[r];
                snow.clear();
            } else if (grid[r][c] == 'S') {
                int row = r + T;
                if (t >= T) row = min(row, barrier - 1 - (int) snow.size());
                else row = min({row, barrier - 1 - (int) snow.size() + T - t, prev - 1});

                snow.emplace_back(row);
                after[row][c] = 'S';
                prev = row;
            }
    }

    for (auto &row : after) cout << row << "\n";
}
