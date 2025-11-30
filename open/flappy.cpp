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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull {
    deque<Point<T>> dq;

    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (dq.size() > 1 && violates(dq[1], dq[0], p)) dq.pop_front();
        dq.emplace_front(p);
    }

    int size() const {
        return dq.size();
    }

    auto & operator[](int i) {
        return dq[i];
    }

    const auto & operator[](int i) const {
        return dq[i];
    }

    auto & back() {
        return dq.back();
    }

    void pop_back() {
        dq.pop_back();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    Point<long long> s, t;
    cin >> s.x >> s.y >> t.x >> t.y >> n;

    MonotonicHull<long long, 1, true> lower;
    MonotonicHull<long long, -1, true> upper;
    auto add = [&](const auto &l, const auto &u) {
        lower.add(l);
        upper.add(u);

        while (lower.size() > 1 && upper.size() > 1 && cross(lower.back(), lower[lower.size() - 2], upper[upper.size() - 2]) <= 0) {
            cout << lower.back().x << " " << lower.back().y << "\n";
            if (lower.size() > 2) {
                lower.pop_back();
                upper.back() = lower.back();
            } else if (upper.size() > 2) {
                upper.pop_back();
                lower.back() = upper.back();
            } else break;
        }
    };
    add(s, s);
    while (n--) {
        long long x, y1, y2;
        cin >> x >> y1 >> y2;

        add(Point(x, y1), Point(x, y2));
    }
    add(t, t);
    cout << t.x << " " << t.y;
}
