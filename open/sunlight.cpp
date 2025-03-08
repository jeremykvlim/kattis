#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-5;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
void add(deque<Point<T>> &half_hull, Point<T> p, bool collinear = false) {
    auto clockwise = [&]() {
        T cross_product = cross(half_hull[1], half_hull[0], p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    while (half_hull.size() > 1 && clockwise()) half_hull.pop_front();
    half_hull.emplace_front(p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> hours(n, 12);
    vector<Point<long long>> points(n, {0, 0}), points_reflected(n, {0, 0});
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;

        points_reflected[i] = {-points[i].x, points[i].y};
    }

    deque<Point<long long>> W_to_E, E_to_W;
    for (int i = 0; i < n; i++) {
        add(W_to_E, points_reflected[i]);
        if (W_to_E.size() > 1 && W_to_E[1].y > points_reflected[i].y) hours[i] -= angle(W_to_E[1] - points_reflected[i]) * 12 / M_PI;
    }
    for (int i = n - 1; ~i; i--) {
        add(E_to_W, points[i]);
        if (E_to_W.size() > 1 && E_to_W[1].y > points[i].y) hours[i] -= angle(E_to_W[1] - points[i]) * 12 / M_PI;
    }
    for (int i = 0; i < n; i++) cout << fixed << setprecision(4) << hours[i] << "\n";
}
