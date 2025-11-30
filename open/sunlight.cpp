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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> hours(n, 12);
    vector<Point<long long>> points(n, {0, 0}), reflected(n, {0, 0});
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;

        reflected[i] = {-points[i].x, points[i].y};
    }

    MonotonicHull<long long> W_to_E, E_to_W;
    for (int i = 0; i < n; i++) {
        W_to_E.add(reflected[i]);
        if (W_to_E.size() > 1 && W_to_E[1].y > reflected[i].y) hours[i] -= angle(W_to_E[1] - reflected[i]) * 12 / M_PI;
    }
    for (int i = n - 1; ~i; i--) {
        E_to_W.add(points[i]);
        if (E_to_W.size() > 1 && E_to_W[1].y > points[i].y) hours[i] -= angle(E_to_W[1] - points[i]) * 12 / M_PI;
    }
    for (auto h : hours) cout << fixed << setprecision(4) << h << "\n";
}
