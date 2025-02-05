#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<string> grid(R);
    for (auto &row : grid) cin >> row;

    auto impossible = [&]() {
        cout << "IMPOSSIBLE";
        exit(0);
    };

    array<int, 4> rainbow{1, 1, 1, 1}, daring{1, 1, 1, 1};
    auto print = [&]() {
        for (int rc : rainbow) cout << rc << " ";
        cout << "\n";
        for (int rc : daring) cout << rc << " ";
        exit(0);
    };

    if (((long long) R * C) & 1) impossible();

    int diff = 0;
    for (auto row : grid)
        for (char c : row) diff += c == 'x' ? 1 : -1;

    if ((R == 1 && C == 2) || (R == 2 && C == 1)) {
        if (diff) impossible();
        else if (R == 1) rainbow[3] = daring[3] = 2;
        else rainbow[2] = daring[2] = 2;
        print();
    }

    if (R == 2 && C == 2) {
        if (!diff) rainbow[2] = rainbow[3] = daring[2] = daring[3] = 2;
        else if (adjacent_find(grid.begin(), grid.end(), not_equal_to<>()) == grid.end()) rainbow[3] = daring[2] = daring[3] = 2;
        else impossible();
        print();
    }

    if (R == 1 || C == 1) {
        int n = (R == 1 ? C : R);
        vector<int> arr(n);
        int d = 0;
        for (int i = 0; i < n; i++) {
            char c = (R == 1 ? grid[0][i] : grid[i][0]);
            d += arr[i] = (c == 'x' ? 1 : -1);
        }

        if (d & 1) impossible();

        if (!d) {
            if (R == 1) rainbow[3] = daring[3] = n;
            else rainbow[2] = daring[2] = n;
            print();
        }

        int half = d / 2, sum = 0, j = -1;
        for (int i = 0; i < n; i++) {
            sum += arr[i];
            if (sum == half) {
                j = i + 1;
                break;
            }
        }
        if (!~j) impossible();

        if (R == 1) {
            if (j == 1) {
                rainbow[3] = daring[3] = n;
                daring[1] = 2;
            } else if (j < n - 1) {
                rainbow[3] = daring[3] = n;
                daring[1] = j + 1;
            } else if (j == n - 1) {
                rainbow[3] = n - 1;
                daring[3] = n;
            } else impossible();
        } else {
            if (j == 1) {
                rainbow[2] = daring[2] = n;
                daring[0] = 2;
            } else if (j < n - 1) {
                rainbow[2] = daring[2] = n;
                daring[0] = j + 1;
            } else if (j == n - 1) {
                rainbow[2] = n - 1;
                daring[2] = n;
            } else impossible();
        }
        print();
    }

    bool transposed = false;
    auto g = grid;
    int n = R, m = C;
    if (C < 3) {
        transposed = true;
        vector<string> transpose(C, string(R, ' '));
        for (int i = 0; i < C; i++)
            for (int j = 0; j < R; j++) transpose[i][j] = grid[j][i];

        g = transpose;
        swap(n, m);
    }

    int d = 0;
    for (auto row : g)
        for (char c : row) d += c == 'x' ? 1 : -1;

    if (d & 1) impossible();

    if (!d) {
        rainbow[2] = daring[2] = n;
        rainbow[3] = daring[3] = m;
        print();
    }

    int half = d / 2, sum = 0, r = -1, c = -1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            sum += g[i][j] == 'x' ? 1 : -1;
            if (sum == half) {
                r = i + 1;
                c = j + 1;
                goto done;
            }
        }
    impossible();

    done:;
    if (r == 1) {
        if (c == 1) {
            rainbow[3] = daring[3] = m;
            daring[1] = 2;
        } else {
            rainbow[3] = c;
            daring[2] = n;
            daring[3] = m;
        }
    } else {
        if (c == 1) {
            rainbow[2] = r;
            daring[1] = 2;
            daring[2] = r - 1;
            daring[3] = m;
        } else {
            rainbow[2] = r - 1;
            rainbow[3] = m;
            daring[0] = daring[2] = r;
            daring[3] = c;
        }
    }

    if (transposed) {
        swap(rainbow[0], rainbow[1]);
        swap(rainbow[2], rainbow[3]);
        swap(daring[0], daring[1]);
        swap(daring[2], daring[3]);
    }
    print();
}
