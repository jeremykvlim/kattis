#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    Point operator-(Point p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
T dot(Point<T> a, Point<T> b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
int sgn(T v) {
    return fabs(v) < 1e-8 ? 0 : (v > 0) - (v < 0);
}

template <typename T>
pair<bool, bool> point_in_polygon(vector<Point<T>> polygon, Point<T> p) {
    bool in = false;
    for (int i = 0; i < polygon.size(); i++) {
        auto a = polygon[i] - p, b = polygon[(i + 1) % polygon.size()] - p;
        if (a.y > b.y) swap(a, b);
        if (sgn(a.y) <= 0 && 0 < sgn(b.y) && sgn(cross(a, b)) < 0) in = !in;
        if (!sgn(cross(a, b)) && sgn(dot(a, b)) <= 0) return {false, true};
    }
    return {in, false};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<int>> polygon(n);
        for (auto &[x, y] : polygon) cin >> x >> y;

        int m;
        cin >> m;

        while (m--) {
            Point<int> p;
            cin >> p.x >> p.y;

            auto [in, on] = point_in_polygon(polygon, p);
            cout << (in ? "in\n" : on ? "on\n" : "out\n");
        }
    }
}
