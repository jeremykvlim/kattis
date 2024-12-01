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
double angle(Point<T> p) {
    return atan2(p.y, p.x);
}

template <typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

template <typename T>
void add(deque<Point<T>> &half_hull, Point<T> p) {
    while (half_hull.size() > 1 && cross(p, half_hull[1], half_hull[0]) > 0) half_hull.pop_front();
    half_hull.emplace_front(p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> hours(n, 12);
    vector<Point<long long>> points(n, {0, 0}), points_reflected(n, {0, 0});
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;

        points_reflected[i] = {-points[i].x, points[i].y};
    }

    deque<Point<long long>> W_to_E, E_to_W;
    for (int i = 0; i < n; i++) {
        add(W_to_E, points_reflected[i]);
        if (W_to_E.size() > 1 && W_to_E[1].y > points_reflected[i].y) hours[i] -= angle(W_to_E[1] - points_reflected[i]) * 12 / M_PI;
    }
    for (int i = n - 1; ~i; i--) {
        add(E_to_W, points[i]);
        if (E_to_W.size() > 1 && E_to_W[1].y > points[i].y) hours[i] -= angle(E_to_W[1] - points[i]) * 12 / M_PI;
    }
    for (int i = 0; i < n; i++) cout << fixed << setprecision(4) << hours[i] << "\n";
}
