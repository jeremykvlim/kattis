#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int g;
    cin >> g;

    vector<pair<int, int>> positions(g);
    pair<int, int> biggest{-1, -1};
    for (auto &[x, y] : positions) {
        cin >> x >> y;

        biggest = {max(biggest.first, x), max(biggest.second, y)};
    }

    int m;
    cin >> m;

    auto hypot = [](int a, int b) {
        return sqrt(a * a + b * b);
    };

    vector<vector<bool>> soaked(biggest.first + 1, vector<bool>(biggest.second + 1, false));
    while (m--) {
        int x, y, r;
        cin >> x >> y >> r;

        for (int i = max(x - r, 0); i <= min(x + r, biggest.first); i++)
            for (int j = max(y - r, 0); j <= min(y + r, biggest.second); j++)
                if (hypot(i - x, j - y) <= r) soaked[i][j] = true;
    }

    for (auto [x, y] : positions)
        if (soaked[x][y]) g--;

    cout << g;
}
