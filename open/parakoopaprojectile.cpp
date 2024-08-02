#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }
};

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;
    sort(points.begin(), points.end());

    auto dist = 1e18;
    for (int i = 1; i < n; i++) {
        auto [x1, y1] = points[i - 1];
        auto [x2, y2] = points[i];
        if (x1 != x2 && cross(points[i], points[i - 1]) > 0) dist = min(dist, (y1 * x2 * x2 - y2 * x1 * x1) / cross(points[i], points[i - 1]));
    }

    cout << fixed << setprecision(6) << dist;
}
