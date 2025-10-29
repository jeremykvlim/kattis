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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        T cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    for (auto p : points) {
        while (convex_hull.size() > 1 && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    convex_hull.pop_back();
    return convex_hull;
}

template <typename T>
pair<T, array<T, 4>> minimum_bounding_rectangle(vector<Point<T>> &points) {
    T area = numeric_limits<T>::infinity();
    array<T, 4> rectangle;

    auto convex_hull = monotone_chain(points);
    int n = convex_hull.size();
    auto next = [&](int i) {
        return (i + 1) % n;
    };

    T xl = numeric_limits<T>::infinity(), xr = numeric_limits<T>::lowest(),
      yd = numeric_limits<T>::infinity(), yu = numeric_limits<T>::lowest();
    for (int i = 0, l = -1, r = -1, d = -1, u = -1; i < n; i++) {
        auto v = convex_hull[next(i)] - convex_hull[i];
        auto len = euclidean_dist(v);
        v /= len;

        auto w = ~v;
        if (!i) {
            for (int j = 0; j < n; j++) {
                auto x = dot(convex_hull[j], v), y = dot(convex_hull[j], w);
                if (xl > x) {
                    xl = x;
                    l = j;
                }
                if (xr < x) {
                    xr = x;
                    r = j;
                }
                if (yd > y) {
                    yd = y;
                    d = j;
                }
                if (yu < y) {
                    yu = y;
                    u = j;
                }
            }
        } else {
            while (dot(convex_hull[l], v) > dot(convex_hull[next(l)], v)) l = next(l);
            while (dot(convex_hull[r], v) < dot(convex_hull[next(r)], v)) r = next(r);
            while (dot(convex_hull[d], w) > dot(convex_hull[next(d)], w)) d = next(d);
            while (dot(convex_hull[u], w) < dot(convex_hull[next(u)], w)) u = next(u);
            xl = dot(convex_hull[l], v);
            xr = dot(convex_hull[r], v);
            yd = dot(convex_hull[d], w);
            yu = dot(convex_hull[u], w);
        }

        auto a = (xr - xl) * (yu - yd);
        if (area > a) {
            area = a;
            rectangle = {xl, xr, yd, yu};
        }
    }
    return {area, rectangle};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<long double>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;
    cout << fixed << setprecision(5) << minimum_bounding_rectangle(coords).first;
}
