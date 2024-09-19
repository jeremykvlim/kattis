#include <bits/stdc++.h>
using namespace std;

template<typename T>
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

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template<typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

template<typename T>
deque<Point<T>> monotone(vector<Point<T>> &points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return deque<Point<T>>(points.begin(), points.end());

    deque<Point<T>> convex_hull;
    for (auto p: points) {
        while (convex_hull.size() > 1 && cross(convex_hull[1], convex_hull[0], p) <= 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p: points) {
        while (convex_hull.size() > s && cross(convex_hull[1], convex_hull[0], p) <= 0) convex_hull.pop_front();
        convex_hull.emplace_front(p);
    }

    convex_hull.pop_front();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<Point<int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;

        auto convex_hull = monotone(points);
        int size = convex_hull.size();
        
        auto next = [&](int i) {
            return (i + 1) % size;
        };

        int area = 0;
        for (int s = 0; s < size; s++) 
            for (int i = 0, j = s, k = 0, l = s; i < size; i++, j = next(j)) {
                while (k != j && abs(cross(convex_hull[next(k)] - convex_hull[i], convex_hull[j] - convex_hull[i]))
                              >= abs(cross(convex_hull[k] - convex_hull[i], convex_hull[j] - convex_hull[i])))
                    k = next(k);

                while (l != i && abs(cross(convex_hull[next(l)] - convex_hull[j], convex_hull[i] - convex_hull[j]))
                              >= abs(cross(convex_hull[l] - convex_hull[j], convex_hull[i] - convex_hull[j])))
                    l = next(l);
                
                area = max(area, abs(cross(convex_hull[k] - convex_hull[i], convex_hull[j] - convex_hull[i])) + abs(cross(convex_hull[l] - convex_hull[j], convex_hull[i] - convex_hull[j])));
            }

        cout << area / 2 << (area & 1 ? ".5\n" : "\n");
    }
}
