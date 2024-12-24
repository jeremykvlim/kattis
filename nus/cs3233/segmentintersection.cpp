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

    auto operator<=(const Point &p) const {
        return *this < p || *this == p;
    }

    Point operator+(Point p) const {
        return {x + p.x, y + p.y};
    }

    Point operator-(Point p) const {
        return {x - p.x, y - p.y};
    }

    Point operator*(T c) const {
        return {c * x, c * y};
    }

    Point operator/(T c) const {
        return {x / c, y / c};
    }
};

template <typename T>
T dot(Point<T> a, Point<T> b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
int sgn(T v) {
    return fabs(v) < 1e-8 ? 0 : (v > 0) - (v < 0);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
    Line(T &a, T &b, T &c) {
        if (fabs(a) < 1e-8 && fabs(b) > 1e-8) {
            a = {0, c / b};
            b = {1, c / b};
        } else if (fabs(b) < 1e-8) {
            a = {c / a, 0};
            b = {c / a, 1};
        } else if (fabs(c) < 1e-8) {
            a = {0, c / b};
            b = {1, (c - a) / b};
        } else {
            a = {0, c / b};
            b = {c / a, 0};
        }
    }
};

template <typename T>
bool point_on_line(Point<T> p, Line<T> l) {
    return !sgn(cross(l.b - l.a, p - l.a)) && dot(l.a - p, l.b - p) <= 0;
}

template <typename T>
bool parallel(Line<T> l1, Line<T> l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
bool collinear(Line<T> l1, Line<T> l2) {
    return parallel(l1, l2) && !sgn(cross(l1.b - l1.a, l2.a - l1.a));
}

template <typename T>
bool intersects(Line<T> l1, Line<T> l2) {
    return collinear(l1, l2) || !parallel(l1, l2);
}

template <typename T>
vector<Point<T>> collinear_overlap(Line<T> l1, Line<T> l2) {
    auto p1 = min(l1.a, l1.b), p2 = max(l1.a, l1.b), p3 = min(l2.a, l2.b), p4 = max(l2.a, l2.b);
    if (p3 <= p2 && p1 <= p4) {
        auto l = max(p1, p3), r = min(p2, p4);
        if (l == r) return {l};
        return {l, r};
    }
    return {};
}

template <typename T>
Point<T> non_parallel_intersection(Line<T> l1, Line<T> l2) {
    return l2.a + (l2.b - l2.a) * cross(l1.b - l1.a, l1.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
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
                if (p1 == p3) cout << fixed << setprecision(2) << p1.x << " " << p1.y << "\n";
                else cout << "none\n";
                return true;
            } else if (p1 == p2) {
                Line<double> l(p3, p4);
                if (point_on_line(p1, l)) cout << fixed << setprecision(2) << p1.x << " " << p1.y << "\n";
                else cout << "none\n";
                return true;
            } else if (p3 == p4) {
                Line<double> l(p1, p2);
                if (point_on_line(p3, l)) cout << fixed << setprecision(2) << p3.x << " " << p3.y << "\n";
                else cout << "none\n";
                return true;
            }
            return false;
        };

        auto handle_lines = [&]() {
            Line<double> l1(p1, p2), l2(p3, p4);

            if (!intersects(l1, l2)) cout << "none\n";
            else if (collinear(l1, l2)) {
                auto v = collinear_overlap(l1, l2);
                if (v.empty()) cout << "none\n";
                else {
                    for (auto [x, y] : v) cout << fixed << setprecision(2) << x << " " << y << " ";
                    cout << "\n";
                }
            } else {
                auto i = non_parallel_intersection(l1, l2), l = max(min(p1, p2), min(p3, p4)), r = min(max(p1, p2), max(p3, p4));
                if (l <= i && i <= r) cout << fixed << setprecision(2) << i.x << " " << i.y << "\n";
                else cout << "none\n";
            }
        };

        if (!handle_points()) handle_lines();
    }
}
