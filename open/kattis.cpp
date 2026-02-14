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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull : deque<Point<T>> {
    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (this->size() > 1 && violates((*this)[1], (*this)[0], p)) this->pop_front();
        this->emplace_front(p);
    }
};

int kattis(int n, int h, int x[], int y[], int z[]) {
    vector<pair<double, double>> intervals;
    auto x_intercept = [&](double X1, double Y1, double X2, double Y2) -> double {
        return X1 - Y1 * (X2 - X1) / (Y2 - Y1);
    };
    
    MonotonicHull<long long, 1, true> lower;
    for (int i = 0; i < n; i++) {
        lower.add(Point<long long>(x[i], y[i]));
        if (z[i]) {
            double l = 0, r = x[n - 1];
            if (lower.size() > 1 && lower[1].y > lower[0].y) l = max(l, x_intercept(lower[1].x, lower[1].y - h, lower[0].x, lower[0].y - h));
            intervals.emplace_back(l, r);
        }
    }

    MonotonicHull<long long, -1, true> upper;
    for (int i = n - 1, j = intervals.size() - 1; ~i && ~j; i--) {
        upper.add(Point<long long>(x[i], y[i]));
        if (z[i]) {
            if (upper.size() > 1 && upper[1].y > upper[0].y) intervals[j].second = min(intervals[j].second, x_intercept(upper[1].x, upper[1].y - h, upper[0].x, upper[0].y - h));
            j--;
        }
    }
    sort(intervals.begin(), intervals.end(), [](const auto &i1, const auto &i2) { return i1.second < i2.second; });

    int guards = 0;
    auto prev = -1e20;
    for (auto [l, r] : intervals)
        if (prev < l) {
            prev = r;
            guards++;
        }
    return guards;
}
