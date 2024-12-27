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

    vector<Point<long long>> points;
    while (n--) {
        int x, y;
        char c;
        cin >> x >> y >> c;

        if (c == 'Y') points.emplace_back(x, y);
    }

    auto convex_hull = monotone_chain(points);
    cout << convex_hull.size() << "\n";
    for (auto [x, y] : convex_hull) cout << x << " " << y << "\n";
}
