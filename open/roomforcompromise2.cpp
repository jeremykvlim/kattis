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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

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

    bool contains(const Circle &c) const {
        return sgn(euclidean_dist(origin, c.origin) - radius + c.radius) < 0;
    }

    bool borders(const Circle &c) const {
        return !sgn(euclidean_dist(origin, c.origin) - radius + c.radius);
    }

    bool encloses(const Circle &c) const {
        return contains(c) || borders(c);
    }
};

template <typename T>
T area_of_circle_circle_intersections(const vector<Circle<T>> &circles) {
    int n = circles.size();

    T area = 0;
    for (int i = 0; i < n; i++) {
        vector<pair<T, T>> intervals{{0, 2 * M_PI}};
        for (int j = 0; j < n; j++)
            if (i != j) {
                auto v = circles[j].origin - circles[i].origin;
                T d = euclidean_dist(v);
                if (!sgn(d)) {
                    if (circles[j].encloses(circles[i])) continue;
                    intervals.clear();
                    break;
                }

                T bound = (circles[i].radius * circles[i].radius + d * d - circles[j].radius * circles[j].radius) / (2 * circles[i].radius * d);
                if (bound <= -1) continue;
                if (bound >= 1) {
                    intervals.clear();
                    break;
                }

                auto add = [&](const pair<T, T> &interval) {
                    vector<pair<T, T>> merged;
                    for (auto [l, r] : intervals) {
                        T x = max(l, interval.first), y = min(r, interval.second);
                        if (x < y) merged.emplace_back(x, y);
                    }
                    return merged;
                };

                T a = angle(v), theta = acos(bound),
                  l = fmod(fmod(a - theta, 2 * M_PI) + 2 * M_PI, 2 * M_PI), r = fmod(fmod(a + theta, 2 * M_PI) + 2 * M_PI, 2 * M_PI);

                if (l <= r) intervals = add({l, r});
                else {
                    auto temp = add({l, 2 * M_PI});
                    intervals = add({0, r});
                    intervals.insert(intervals.end(), temp.begin(), temp.end());
                }
                if (intervals.empty()) break;
            }

        for (auto [l, r] : intervals)
            if (l < r) {
                auto [p, R] = circles[i];
                area += 0.5 * (R * p.x * (sin(r) - sin(l)) + R * p.y * (cos(l) - cos(r)) + R * R * (r - l));
            }
    }
    return area;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Circle<long double>> circles(n);
    for (auto &[p, r] : circles) cin >> p.x >> p.y >> r;
    cout << fixed << setprecision(4) << area_of_circle_circle_intersections(circles);
}
