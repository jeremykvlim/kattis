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
vector<pair<Point<T>, int>> monotone_chain(vector<pair<Point<T>, int>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<pair<Point<T>, int>> convex_hull;

    auto clockwise = [&](auto p) {
        auto cross_product = cross(convex_hull[convex_hull.size() - 2].first, convex_hull.back().first, p.first);
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
    double k;
    cin >> n >> k;

    vector<double> h(n + 1);
    vector<pair<Point<double>, int>> points;
    points.emplace_back(Point<double>(0, 0), 0);
    for (int i = 1; i <= n; i++) {
        cin >> h[i];

        h[i] += k * i * (i - n - 1);
        if (h[i] >= 0) points.emplace_back(Point<double>(i, h[i]), points.size());
    }
    points.emplace_back(Point<double>(n + 1, 0), points.size());

    auto convex_hull = monotone_chain(points);
    sort(convex_hull.begin(), convex_hull.end(), [&](auto p1, auto p2) {return p1.second < p2.second;});

    for (int i = 1, j = 0; i <= n; i++) {
        while (j < convex_hull.size() && points[convex_hull[j].second].first.x < i) j++;
        if (points[convex_hull[j].second].first.x == i) continue;

        int l = points[convex_hull[j - 1].second].first.x, r = points[convex_hull[j].second].first.x;
        h[i] = h[l] + (h[r] - h[l]) / (r - l) * (i - l);
    }

    double height = 0;
    for (int i = 1; i <= n; i++) {
        h[i] -= k * i * (i - n - 1);
        height = max(height, h[i]);
    }
    cout << fixed << setprecision(6) << height;
}
