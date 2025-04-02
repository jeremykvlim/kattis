#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(U x, U y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    const auto begin() const {
        return &x;
    }

    const auto end() const {
        return &y + 1;
    }

    Point operator-() const {
        return {-x, -y};
    }

    bool operator<(const Point &p) const {
        return !approximately_equal(x, p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y);
        return x != p.x || y != p.y;
    }

    bool operator<=(const Point &p) const {
        return *this < p || *this == p;
    }

    bool operator>=(const Point &p) const {
        return *this > p || *this == p;
    }

    Point operator+(const Point &p) const {
        return {x + p.x, y + p.y};
    }

    Point operator+(const T &v) const {
        return {x + v, y + v};
    }

    Point & operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point & operator+=(const T &v) {
        x += v;
        y += v;
        return *this;
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }

    Point operator-(const T &v) const {
        return {x - v, y - v};
    }

    Point & operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point & operator-=(const T &v) {
        x -= v;
        y -= v;
        return *this;
    }

    Point operator*(const T &v) const {
        return {x * v, y * v};
    }

    Point & operator*=(const T &v) {
        x *= v;
        y *= v;
        return *this;
    }

    Point operator/(const T &v) const {
        return {x / v, y / v};
    }

    Point & operator/=(const T &v) {
        x /= v;
        y /= v;
        return *this;
    }
};

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        T cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
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
