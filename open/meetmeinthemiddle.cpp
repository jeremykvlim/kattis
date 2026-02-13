#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
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

    Point operator~() const {
        return {-y, x};
    }

    bool operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}

    bool operator<(const Circle &c) const {
        return radius != c.radius ? radius < c.radius : origin < c.origin;
    }

    bool operator==(const Circle &c) const {
        return origin == c.origin && radius == c.radius;
    }

    bool encloses(const Circle &c) const {
        return euclidean_dist(origin, c.origin) <= radius - c.radius;
    }
};

template <typename T>
vector<Point<T>> circle_circle_intersections(const Circle<T> &c1, const Circle<T> &c2) {
    if (c1 == c2) return {};

    T d = euclidean_dist(c1.origin, c2.origin);
    if (d > c1.radius + c2.radius || d < abs(c1.radius - c2.radius)) return {};

    T r1_sq = c1.radius * c1.radius, r2_sq = c2.radius * c2.radius,
      l = (d * d + r1_sq - r2_sq) / (2 * d);

    auto v = c2.origin - c1.origin, p = c1.origin + v * l / d;
    if (d == c1.radius + c2.radius) return {p};

    T h = sqrt(r1_sq - l * l);
    auto u = ~v, q = u * h / d;
    return {p + q, p - q};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Circle<double> c1, c2;
    cin >> c1.origin.x >> c1.origin.y >> c1.radius >> c2.origin.x >> c2.origin.y >> c2.radius;

    if (c1.encloses(c2)) {
        cout << "their";
        exit(0);
    }

    if (c2.encloses(c1)) {
        cout << "our";
        exit(0);
    }

    auto points = circle_circle_intersections(c1, c2);
    if (points.empty()) cout << "impossible";
    else if (points.size() == 1) cout << fixed << setprecision(6) << "compromise\n" << points[0].x << " " << points[0].y;
    else {
        auto p1 = points[0], p2 = points[1];
        if (p1 > p2) swap(p1, p2);
        cout << fixed << setprecision(6) << "compromises\n" << p1.x << " " << p1.y << "\n" << p2.x << " " << p2.y;
    }
}
