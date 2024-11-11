#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

template <typename T>
void add(deque<Point<T>> &half_hull, Point<T> p) {
    while (half_hull.size() > 1 && cross(half_hull[1], p, half_hull[0]) > 0) half_hull.pop_front();
    half_hull.emplace_front(p);
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

    deque<Point<double>> half_hull;
    for (auto p : points) add(half_hull, p);
    for (int i = half_hull.size() - 1; i; i--) {
        int l = half_hull[i].x, r = half_hull[i - 1].x;

        for (int j = l; j < r; j++) houses[j] = (houses[l] * (r - j) + houses[r] * (j - l)) / (r - l);
    }

    double height = 0;
    for (int i = 1; i <= n; i++) {
        houses[i] += k * i * (n + 1 - i);
        height = max(height, houses[i]);
    }

    cout << fixed << setprecision(6) << height;
}
