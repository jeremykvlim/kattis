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
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        auto cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    for (auto p : points) {
        while (convex_hull.size() > 1 && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    convex_hull.pop_back();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    auto convex_hull = monotone_chain(points, true);

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
