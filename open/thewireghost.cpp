#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
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

    bool operator<(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == -1 : sgn(y - p.y) == -1;
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == 1 : sgn(y - p.y) == 1;
        return x != p.x ? x > p.x : y > p.y;
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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
bool point_on_line(const Point<T> &p, const Line<T> &l, bool include_endpoints = true) {
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double L;
    int n;
    cin >> L >> n;

    vector<pair<double, double>> angles(100);
    for (int k = 1; k <= 100; k++) {
        auto theta = M_PI_2 * k / 100;
        angles[k - 1] = {sin(theta), cos(theta)};
    }

    vector<pair<double, Line<double>>> segments{{L, {{0., 0.}, {L, 0.}}}};
    while (n--) {
        double x;
        char c;
        cin >> x >> c;

        int bend = 0;
        double total = 0;
        vector<pair<double, Line<double>>> l, r;
        while (bend < segments.size() && total + segments[bend].first < x) {
            l.emplace_back(segments[bend]);
            total += segments[bend++].first;
        }

        if (bend == segments.size()) {
            cout << "GHOST";
            exit(0);
        }

        x -= total;
        if (!sgn(x)) r.emplace_back(segments[bend]);
        else if (!sgn(x - segments[bend].first)) l.emplace_back(segments[bend]);
        else {
            auto [len, line] = segments[bend];
            auto v = line.b - line.a;
            Point<double> split{line.a.x + x / len * v.x, line.a.y + x / len * v.y};
            l.emplace_back(x, Line(line.a, split));
            r.emplace_back(len - x, Line(split, line.b));
        }
        for (int i = bend + 1; i < segments.size(); i++) r.emplace_back(segments[i]);

        segments = l;
        if (r.empty()) continue;
        
        auto pivot = r[0].second.a;
        auto rotate = [&](const Line<double> &l1, double sin = 1, double cos = 0) {
            Line<double> l2;
            bool first = true;
            for (auto p : {l1.a, l1.b}) {
                auto v = p - pivot;
                Point<double> q{pivot.x + cos * v.x, pivot.y + cos * v.y};
                if (c == 'C') q += {sin * v.y, -sin * v.x};
                else q += {-sin * v.y, sin * v.x};
                (first ? l2.a : l2.b) = q;
                first = false;
            }
            return l2;
        };

        for (auto [sin, cos] : angles)
            for (int i = 0; i < l.size(); i++)
                for (int j = 0; j < r.size(); j++) {
                    auto l1 = l[i].second, l2 = rotate(r[j].second, sin, cos);
                    if (!intersects(l1, l2)) continue;
                    else if (collinear(l1, l2)) {
                        auto v = collinear_overlap(l1, l2);
                        if (v.size() == 1 && v[0] != pivot) {
                            cout << "GHOST";
                            exit(0);
                        }
                    } else {
                        auto p = non_collinear_intersection(l1, l2);
                        if (point_on_line(p, l1) && point_on_line(p, l2))
                            if (p != pivot) {
                                cout << "GHOST";
                                exit(0);
                            }
                    }
                }

        for (auto [len, line] : r) segments.emplace_back(len, rotate(line));
    }
    cout << "SAFE";
}
