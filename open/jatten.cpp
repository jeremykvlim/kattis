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
double squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y);
}

template <typename T>
bool collinear(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return approximately_equal(cross(a, b, c), (T) 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    Point<int> p1, p2;;
    cin >> n >> m >> p1.x >> p1.y >> p2.x >> p2.y;

    auto obtuse = [&](const Point<int> &a, const Point<int> &b, const Point<int> &c) -> bool {
        auto d1 = squared_dist(a, b), d2 = squared_dist(a, c), d3 = squared_dist(b, c);
        return (d1 + d2 < d3) || (d1 + d3 < d2) || (d2 + d3 < d1);
    };

    for (int dx = -1e3; dx <= 1e3; dx++)
        for (int dy = -1e3; dy <= 1e3; dy++) {
            auto check = [&](int x3, int y3) {
                if (!collinear(p1, p2, {x3, y3}) && 0 <= x3 && x3 < n && 0 <= y3 && y3 < m && obtuse(p1, p2, {x3, y3})) {
                    cout << x3 << " " << y3;
                    exit(0);
                }
            };
            check(p1.x + dx, p1.y + dy);
            check(p2.x + dx, p2.y + dy);
        }
}
