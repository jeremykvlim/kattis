#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point<T> &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(Point<T> &p) const {
        return x == p.x && y == p.y;
    }

    Point operator-(Point<T> &p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
double cross(Point<T> a, Point<T> b) {
    return (double) (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

template <typename T>
deque<Point<T>> half_hull(vector<Point<T>> &points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    deque<Point<T>> convex_hull;
    for (auto p : points) {
        while (convex_hull.size() > 1 && cross(convex_hull[1], convex_hull[0], p) > 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }
    reverse(convex_hull.begin(), convex_hull.end());
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<Point<long long>> e(r), n(c);
    auto read = [](vector<Point<long long>> &d, int size) {
        for (int i = 0; i < size; i++) {
            long long s;
            cin >> s;

            d[i] = {i, s};
        }
    };
    read(e, r);
    read(n, c);

    auto east = half_hull(e), north = half_hull(n);
    vector<pair<Point<long long>, char>> route;
    for (int i = 1; i < east.size(); i++) route.emplace_back(east[i] - east[i - 1], 'N');
    for (int i = 1; i < north.size(); i++) route.emplace_back(north[i] - north[i - 1], 'E');
    sort(route.begin(), route.end(), [&](auto p1, auto p2) {return cross(p1.first, p2.first) > 0;});

    for (auto [point, direction] : route)
        while (point.x--) cout << direction;
}
