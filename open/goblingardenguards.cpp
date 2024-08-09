#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
double dist(Point<T> a, Point<T> b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int g;
    cin >> g;

    vector<Point<int>> points(g);
    pair<int, int> farthest{-1, -1};
    for (auto &[x, y] : points) {
        cin >> x >> y;

        farthest = {max(farthest.first, x), max(farthest.second, y)};
    }

    int m;
    cin >> m;

    vector<vector<bool>> soaked(farthest.first + 1, vector<bool>(farthest.second + 1, false));
    while (m--) {
        int x, y, r;
        cin >> x >> y >> r;

        for (int i = max(x - r, 0); i <= min(x + r, farthest.first); i++)
            for (int j = max(y - r, 0); j <= min(y + r, farthest.second); j++)
                if (dist(Point(i, j), Point(x, y)) <= r) soaked[i][j] = true;
    }

    for (auto [x, y] : points)
        if (soaked[x][y]) g--;

    cout << g;
}
