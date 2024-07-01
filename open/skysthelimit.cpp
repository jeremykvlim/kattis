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
};

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

template <typename T>
deque<Point<T>> half_hull(vector<Point<T>> &points) {
    deque<Point<T>> convex_hull;
    for (auto p : points) {
        while (convex_hull.size() > 1 && cross(convex_hull[1], p, convex_hull[0]) > 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }
    
    reverse(convex_hull.begin(), convex_hull.end());
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double k;
    cin >> n >> k;

    vector<double> houses(n + 2, 0);
    vector<Point<double>> points(n + 2);
    points[0] = {0, 0};
    points[n + 1] = {(double) n + 1, 0};
    for (int i = 1; i <= n; i++) {
        cin >> houses[i];

        houses[i] -= k * i * (n + 1 - i);
        points[i] = {(double) i, houses[i]};
    }

    auto convex_hull = half_hull(points);
    for (int i = 0; i < convex_hull.size() - 1; i++) {
        int l = convex_hull[i].x, r = convex_hull[i + 1].x;

        for (int j = l; j < r; j++) houses[j] = (houses[l] * (r - j) + houses[r] * (j - l)) / (r - l);
    }

    double height = 0;
    for (int i = 1; i <= n; i++) {
        houses[i] += k * i * (n + 1 - i);
        height = max(height, houses[i]);
    }

    cout << fixed << setprecision(6) << height;
}
