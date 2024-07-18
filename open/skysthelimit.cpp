#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

template <typename T>
void add(deque<Point<T>> &trick_hull, Point<T> p) {
    while (trick_hull.size() > 1 && cross(trick_hull[1], p, trick_hull[0]) > 0) trick_hull.pop_front();
    trick_hull.emplace_front(p);
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

    deque<Point<double>> trick_hull;
    for (auto p : points) add(trick_hull, p);
    for (int i = trick_hull.size() - 1; i; i--) {
        int l = trick_hull[i].x, r = trick_hull[i - 1].x;

        for (int j = l; j < r; j++) houses[j] = (houses[l] * (r - j) + houses[r] * (j - l)) / (r - l);
    }

    double height = 0;
    for (int i = 1; i <= n; i++) {
        houses[i] += k * i * (n + 1 - i);
        height = max(height, houses[i]);
    }

    cout << fixed << setprecision(6) << height;
}
