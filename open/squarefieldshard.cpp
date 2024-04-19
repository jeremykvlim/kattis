#include <bits/stdc++.h>
using namespace std;

void backtrack(int i, int squares, int curr, int &y, vector<pair<int, int>> &points, vector<pair<pair<int, int>, pair<int, int>>> &minmax) {
    if (squares < minmax.size() && curr <= y) {
        if (i == points.size()) y = min(y, curr);
        else
            for (int j = 0; j <= squares; j++) {
                auto original = minmax[j];
                auto &[p1, p2] = minmax[j];
                p1 = {min(p1.first, points[i].first), min(p1.second, points[i].second)};
                p2 = {max(p2.first, points[i].first), max(p2.second, points[i].second)};

                backtrack(i + 1, max(squares, j + 1), max(curr, max(p2.first - p1.first, p2.second - p1.second)), y, points, minmax);
                minmax[j] = original;
            }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int n, k;
        cin >> n >> k;

        vector<pair<int, int>> points(n);
        for (auto &[X, Y] : points) cin >> X >> Y;

        vector<pair<pair<int, int>, pair<int, int>>> minmax(k + 1, {{INT_MAX, INT_MAX}, {INT_MIN, INT_MIN}});
        int y = INT_MAX;
        backtrack(0, 0, 0, y, points, minmax);
        cout << "Case #" << x << ": " << y << "\n";
    }
}
