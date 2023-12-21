#include <bits/stdc++.h>
using namespace std;

void backtrack(int i, int squares, int curr, int &len, vector<pair<int, int>> &points, vector<pair<pair<int, int>, pair<int, int>>> &minmax) {
    if (squares < minmax.size() && curr <= len) {
        if (i == points.size()) len = min(len, curr);
        else
            for (int j = 0; j <= squares; j++) {
                auto p1 = minmax[j].first, p2 = minmax[j].second;
                minmax[j].first = {min(p1.first, points[i].first), min(p1.second, points[i].second)};
                minmax[j].second = {max(p2.first, points[i].first), max(p2.second, points[i].second)};

                backtrack(i + 1, max(squares, j + 1), max(curr, max(minmax[j].second.first - minmax[j].first.first, minmax[j].second.second - minmax[j].first.second)), 
                          len, points, minmax);
                minmax[j].first = p1, minmax[j].second = p2;
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
        for (auto &p : points) cin >> p.first >> p.second;

        vector<pair<pair<int, int>, pair<int, int>>> minmax(k + 1, {{INT_MAX, INT_MAX}, {INT_MIN, INT_MIN}});
        int len = INT_MAX;
        backtrack(0, 0, 0, len, points, minmax);

        cout << "Case #" << x << ": " << len << "\n";
    }
}
