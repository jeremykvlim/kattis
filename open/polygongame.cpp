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
        return sgn(x - p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return sgn(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !sgn(x - p.x) && !sgn(y - p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return sgn(x - p.x) || sgn(y - p.y);
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
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

template <typename T>
array<vector<Point<T>>, 2> sutherland_hodgman(const vector<Point<T>> &polygon, const Line<T> &clip, double epsilon = 1e-5) {
    int n = polygon.size();

    array<vector<Point<T>>, 2> clipped;
    for (int k = 0; k < 2; k++) {
        auto &p = clipped[k];
        for (int i = 0; i < n; i++) {
            auto a = polygon[i], b = polygon[(i + 1) % n];

            T ca = cross(clip.b - clip.a, a - clip.a), cb = cross(clip.b - clip.a, b - clip.a);
            bool a_inside = k ? ca <= epsilon : ca >= -epsilon, b_inside = k ? cb <= epsilon : cb >= -epsilon;

            Line<T> edge{a, b};
            if (a_inside && b_inside) p.emplace_back(b);
            else if (a_inside) p.emplace_back(non_collinear_intersection(edge, clip));
            else if (b_inside) {
                p.emplace_back(non_collinear_intersection(edge, clip));
                p.emplace_back(b);
            }
        }
    }
    return clipped;
}

template <typename T>
T doubled_signed_area_of_polygon(const vector<Point<T>> &polygon) {
    T area = 0;
    for (int i = 0; i < polygon.size(); i++) area += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
    return area;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Point<double>> original(n);
    for (auto &[x, y] : original) cin >> x >> y;

    vector<Line<double>> lines(m);
    for (auto &[a, b] : lines) cin >> a.x >> a.y >> b.x >> b.y;

    vector<vector<Point<double>>> polygons{original};
    for (auto L : lines) {
        vector<vector<Point<double>>> temp;
        for (auto &p : polygons) {
            auto [l, r] = sutherland_hodgman(p, L);
            if (l.size() > 2 && r.size() > 2) {
                temp.emplace_back(l);
                temp.emplace_back(r);
            } else temp.emplace_back(p);
        }
        polygons = temp;
    }

    double area = 0;
    for (auto &p : polygons) area = max(area, 0.5 * fabs(doubled_signed_area_of_polygon(p)));
    cout << fixed << setprecision(6) << area;
}
