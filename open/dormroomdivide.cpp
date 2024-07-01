#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
double area_of_triangle(Point<T> a, Point<T> b, Point<T> c) {
    return 0.5 * fabs(a.x * b.y + b.x * c.y + c.x * a.y - b.x * a.y - c.x * b.y - a.x * c.y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<double> pref(n - 1, 0);
    double sum = 0;
    for (int i = 1; i < n - 1; i++) pref[i] = sum += area_of_triangle(points[0], points[i], points[i + 1]);

    int j;
    for (int i = 0; i < n - 1; i++)
        if (sum <= 2 * pref[i]) {
            j = i;
            break;
        }

    auto calc = [&](int i) -> array<double, 3> {
        return {points[i].x - points[0].x, points[0].y - points[i].y,
                sum - 2 * pref[i - 1] + points[i].x * points[0].y - points[0].x * points[i].y};
    };

    auto [x1, y1, a1] = calc(j);
    auto [x2, y2, a2] = calc(j + 1);
    auto x = (x1 * a2 - x2 * a1) / (x1 * y2 - x2 * y1), y = (a1 - y1 * x) / x1;

    cout << fixed << setprecision(6) << x << " " << y;
}
