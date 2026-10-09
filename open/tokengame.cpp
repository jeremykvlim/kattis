#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> grundy(301, vector<int>(301, 0)), indices(301, vector<int>(601, -1));
    vector<dynamic_bitset<>> row(301, dynamic_bitset<>(601, 0)), col(301, dynamic_bitset<>(601, 0));
    for (int r = 1; r <= 300; r++)
        for (int c = 1; c <= 300; c++) {
            if (abs(r - c) > 1) grundy[r][c] = (~(row[r] | col[c])).find_first();
            row[r][grundy[r][c]] = col[c][grundy[r][c]] = true;
            if (grundy[r][c]) indices[c][grundy[r][c]] = r;
        }

    int n;
    cin >> n;

    while (n--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        auto count = [&](int r1, int c1, int r2, int c2) {
            if (r2 == c2) return r1 > c1 + 1;
            if (abs(r2 - c2) == 1) return r1 > c1;
            return ~indices[c1][grundy[r2][c2]] && indices[c1][grundy[r2][c2]] < r1;
        };
        cout << count(x1, x2, y1, y2) + count(y1, y2, x1, x2) + count(x2, x1, y2, y1) + count(y2, y1, x2, x1) << "\n";
    }
}