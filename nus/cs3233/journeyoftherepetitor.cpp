#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (auto &row : grid) cin >> row;

    vector<vector<pair<int, int>>> coords(n, vector<pair<int, int>>(m, {-1, -1}));
    coords[0][0] = {0, 0};

    int r = 0, c = 0, R = 0, C = 0;
    auto change = [&](char cell) {
        switch (cell) {
            case 'N':
                if (--r == -1) r = n - 1;
                R--;
                break;
            case 'S':
                if (++r == n) r = 0;
                R++;
                break;
            case 'E':
                if (++c == m) c = 0;
                C++;
                break;
            case 'W':
                if (--c == -1) c = m - 1;
                C--;
                break;
        }
    };
    
    for (int i = 0; i < n * m; i++) {
        change(grid[r][c]);

        if (coords[r][c] != make_pair(-1, -1)) {
            if (coords[r][c] == make_pair(R, C)) {
                cout << 0;
                exit(0);
            }
            break;
        }

        coords[r][c] = {R, C};
    }

    cout << "1\n";
    switch (grid[0][0]) {
        case 'N':
            cout << n << " 1 S";
            break;
        case 'S':
            cout << "2 1 N";
            break;
        case 'E':
            cout << "1 2 W";
            break;
        case 'W':
            cout << "1 " << m << " E";
            break;
    }
}
