#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int X = 1; X <= t; X++) {
        int n, k;
        cin >> n >> k;

        vector<pair<int, int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;

        vector<pair<pair<int, int>, pair<int, int>>> minmax(k + 1, {{INT_MAX, INT_MAX}, {INT_MIN, INT_MIN}});
        int Y = INT_MAX;
        auto dfs = [&](auto &&self, int i = 0, int squares = 0, int len = 0) -> void {
            if (squares < minmax.size() && len <= Y) {
                if (i == points.size()) Y = min(Y, len);
                else
                    for (int j = 0; j <= squares; j++) {
                        auto temp = minmax[j];
                        auto &[p1, p2] = minmax[j];
                        p1 = {min(p1.first, points[i].first), min(p1.second, points[i].second)};
                        p2 = {max(p2.first, points[i].first), max(p2.second, points[i].second)};
                        self(self, i + 1, max(squares, j + 1), max(len, max(p2.first - p1.first, p2.second - p1.second)));
                        minmax[j] = temp;
                    }
            }
        };
        dfs(dfs);
        cout << "Case #" << X << ": " << Y << "\n";
    }
}
