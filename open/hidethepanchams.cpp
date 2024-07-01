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
double dist(Point<T> a, Point<T> b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
double cross(Point<T> a, Point<T> b, Point<T> c) {
    return (double) (c.x - a.x) * (double) (b.y - a.y) - (double) (c.y - a.y) * (double) (b.x - a.x);
}

template <typename T>
deque<Point<T>> monotone(vector<Point<T>> &points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

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

    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<double> d(1 << n, 0), dp(1 << n, 1e18);
    dp[0] = 0;
    for (int i = 0; i < 1 << n; i++) {
        vector<Point<int>> enclosure;
        for (int j = 0; j < n; j++)
            if (i & (1 << j)) enclosure.emplace_back(points[j]);
        if (enclosure.empty()) continue;

        auto convex_hull = monotone(enclosure);
        for (int j = convex_hull.size() - 1, k = 0; k < convex_hull.size(); j = k++)
            d[i] += dist(convex_hull[j], convex_hull[k]);
    }

    for (int i = 1; i < 1 << n; i++)
        for (int j = i; j; --j &= i)
            if (__builtin_popcount(j) >= 3) dp[i] = min(dp[i], dp[i ^ j] + d[j]);

    cout << fixed << setprecision(6) << dp.back();
}
