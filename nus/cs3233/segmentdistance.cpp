#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if constexpr (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    else return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
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

    Point operator!() const {
        return {y, x};
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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
bool point_on_line(const Line<T> &l, const Point<T> &p, bool include_endpoints = true) {
    return !sgn(cross(l.b - l.a, p - l.a)) && (dot(l.a - p, l.b - p) < 0 || (include_endpoints && approximately_equal(dot(l.a - p, l.b - p), (T) 0)));
}

template <typename T>
bool parallel(const Line<T> &l1, const Line<T> &l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
bool collinear(const Line<T> &l1, const Line<T> &l2) {
    return parallel(l1, l2) && !sgn(cross(l1.b - l1.a, l2.a - l1.a));
}

template <typename T>
bool intersects(const Line<T> &l1, const Line<T> &l2) {
    return collinear(l1, l2) || !parallel(l1, l2);
}

template <typename T>
vector<Point<T>> collinear_overlap(const Line<T> &l1, const Line<T> &l2) {
    auto p1 = min(l1.a, l1.b), p2 = max(l1.a, l1.b), p3 = min(l2.a, l2.b), p4 = max(l2.a, l2.b);
    if (p3 <= p2 && p1 <= p4) {
        auto l = max(p1, p3), r = min(p2, p4);
        if (l == r) return {l};
        return {l, r};
    }
    return {};
}

template <typename T>
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

template <typename T>
double point_line_dist(const Point<T> &p, const Line<T> &l) {
    auto v = l.b - l.a;
    return euclidean_dist(l.a + v * clamp(dot(p - l.a, v) / dot(v, v), (T) 0, (T) 1), p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        Point<double> p1, p2, p3, p4;
        cin >> p1.x >> p1.y >> p2.x >> p2.y >> p3.x >> p3.y >> p4.x >> p4.y;

        auto handle_points = [&]() {
            if (p1 == p2 && p3 == p4) {
                cout << fixed << setprecision(2) << euclidean_dist(p1, p3) << "\n";
                return true;
            } else if (p1 == p2) {
                cout << fixed << setprecision(2) << point_line_dist(p1, Line{p3, p4}) << "\n";
                return true;
            } else if (p3 == p4) {
                cout << fixed << setprecision(2) << point_line_dist(p3, Line{p1, p2}) << "\n";
                return true;
            }
            return false;
        };

        auto handle_lines = [&]() {
            Line<double> l1(p1, p2), l2(p3, p4);
            auto dist = [&] {
                return min({point_line_dist(p1, l2), point_line_dist(p2, l2), point_line_dist(p3, l1), point_line_dist(p4, l1)});
            };

            if (!intersects(l1, l2)) cout << fixed << setprecision(2) << dist() << "\n";
            else if (collinear(l1, l2)) cout << fixed << setprecision(2) << (collinear_overlap(l1, l2).empty() ? dist() : 0) << "\n";
            else {
                auto p = non_collinear_intersection(l1, l2);
                cout << fixed << setprecision(2) << (!point_on_line(l1, p) || !point_on_line(l2, p) ? dist() : 0) << "\n";
            }
        };

        if (!handle_points()) handle_lines();
    }
}
