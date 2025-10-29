#include <bits/stdc++.h>
using namespace std;

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
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T area_of_parallelogram(Point<T> a, Point<T> b, Point<T> c) {
    Point<T> u = b - a, v = c - a;
    return abs(cross(u, v));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (;;) {
        array<Point<int>, 4> p;
        for (auto &[x, y] : p) {
            int z;
            cin >> x >> y >> z;

            if (!x && !y && !z) exit(0);
        }
        auto a1 = area_of_parallelogram(p[0], p[1], p[2]),
             a2 = area_of_parallelogram(p[0], p[1], p[3]),
             a3 = area_of_parallelogram(p[0], p[2], p[3]),
             a4 = area_of_parallelogram(p[1], p[2], p[3]);

        if (a1 != a2 + a3 + a4) {
            cout << "NO\n";
            continue;
        }

        if (!a1) cout << ((min({p[0].x, p[1].x, p[2].x}) < p[3].x && p[3].x < max({p[0].x, p[1].x, p[2].x})) ||
                          (min({p[0].y, p[1].y, p[2].y}) < p[3].y && p[3].y < max({p[0].y, p[1].y, p[2].y})) ? "YES\n" : "NO\n");
        else cout << (a2 && a3 && a4 ? "YES\n" : "NO\n");
    }
}
