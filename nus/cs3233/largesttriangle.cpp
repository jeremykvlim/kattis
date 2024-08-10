#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

template <typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

template <typename T>
deque<Point<T>> monotone(vector<Point<T>> &points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return deque<Point<T>>(points.begin(), points.end());

    deque<Point<T>> convex_hull;
    for (auto p : points) {
        while (convex_hull.size() > 1 && cross(convex_hull[1], convex_hull[0], p) <= 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && cross(convex_hull[1], convex_hull[0], p) <= 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    convex_hull.pop_front();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    auto convex_hull = monotone(points);
    reverse(convex_hull.begin(), convex_hull.end());

    int size = convex_hull.size();
    if (size < 3) {
        cout << 0;
        exit(0);
    }

    double area = 0;
    for (int i = 0; i < size; i++)
        for (int j = i + 1, k = j + 1; j < size; j++) {
            while (k % size != i && cross(convex_hull[i], convex_hull[j], convex_hull[(k + 1) % size]) >= cross(convex_hull[i], convex_hull[j], convex_hull[k])) ++k %= size;
            area = max(area, cross(convex_hull[i], convex_hull[j], convex_hull[k % size]));
        }

    cout << fixed << setprecision(5) << area / 2;
}
