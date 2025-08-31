#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c, n;
    cin >> r >> c >> n;

    auto flip = [&](char &ch) -> void {
        ch = ch == '/' ? '\\' : '/';
    };

    if (r == 1) {
        char ch = '/';
        for (int i = 0; i <= c - n; i++) {
            int count = (i < c - n) ? 1 : n;
            while (count--) cout << ch;
            flip(ch);
        }
        cout << "\n";
        exit(0);
    }

    if (c == 1) {
        char ch = '/';
        for (int i = 0; i <= r - n; i++) {
            int count = (i < r - n) ? 1 : n;
            while (count--) cout << ch << "\n";
            flip(ch);
        }
        exit(0);
    }

    vector<string> grid(r, string(c, '/'));
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if ((i + j) & 1) grid[i][j] = '\\';

    for (int j = 1, count = 0; j < r + c - 1 && count < (n - 1) / 2; j += 2, count++)
        for (int i = max(0, j - c + 1); i <= min(r - 1, j); i++) grid[i][j - i] = '/';

    if (!(n & 1)) flip(grid[r - 1][c - 1]);
    for (auto row : grid) cout << row << "\n";
}
