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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
pair<bool, bool> point_in_polygon(const vector<Point<T>> &polygon, const Point<T> &p) {
    bool in = false;
    for (int i = 0; i < polygon.size(); i++) {
        auto a = polygon[i] - p, b = polygon[(i + 1) % polygon.size()] - p;
        if (a.y > b.y) swap(a, b);
        if (sgn(a.y) <= 0 && 0 < sgn(b.y) && sgn(cross(a, b)) < 0) in = !in;
        if (!sgn(cross(a, b)) && sgn(dot(a, b)) <= 0) return {false, true};
    }
    return {in, false};
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Point<double>> toys(n), trees(m);
    for (auto &[x, y] : toys) cin >> x >> y;
    for (auto &[x, y] : trees) cin >> x >> y;

    vector<Point<double>> wrapped{Point(0., 0.)};
    vector<double> angles{0};
    Point<double> spot(0, 0);
    double len = 0;
    for (auto toy : toys)
        while (spot != toy) {
            auto prev = wrapped.back(), a = spot - prev;

            Point<double> tree(0, 0);
            auto smallest = 2 * M_PI;
            for (auto t : trees)
                if (point_in_polygon({prev, spot, toy}, t).first) {
                    auto b = t - prev;

                    auto theta = acos(dot(a, b) / (euclidean_dist(a) * euclidean_dist(b)));
                    if (smallest > theta) {
                        smallest = theta;
                        tree = t;
                    }
                }
            auto curr = tree != Point(0., 0.) ? non_collinear_intersection(Line(prev, tree), Line(spot, toy)) : toy, b = curr - prev;

            auto theta = fmod(angle(a) - angle(b) + 3 * M_PI, 2 * M_PI) - M_PI;
            if (wrapped.size() > 1 && ((angles.back() < 0) ^ (angles.back() < -theta))) {
                spot = non_collinear_intersection(Line(wrapped[wrapped.size() - 2], prev), Line(spot, toy));
                wrapped.pop_back();
                angles.pop_back();
            } else {
                angles.back() += theta;
                spot = curr;
                if (tree != Point(0., 0.)) {
                    wrapped.emplace_back(tree);
                    angles.emplace_back(sgn(theta) * 1e-8);
                }
            }

            double l = 0;
            for (int i = 0; i < wrapped.size(); i++) l += euclidean_dist(wrapped[i], (i + 1 < wrapped.size()) ? wrapped[i + 1] : spot);
            len = max(len, l);
        }

    cout << fixed << setprecision(2) << len;
}
