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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
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
pair<bool, bool> point_in_circle(const Circle<T> &c, const Point<T> &p) {
    auto d = euclidean_dist(p, c.origin) - c.radius;
    return {sgn(d) < 0, !sgn(d)};
}

template <typename T>
vector<Point<T>> circle_circle_intersections(const Circle<T> &c1, const Circle<T> &c2) {
    if (c1 == c2) return {};

    T d = euclidean_dist(c1.origin, c2.origin);
    if (d > c1.radius + c2.radius || d < abs(c1.radius - c2.radius)) return {};

    T r1_sq = c1.radius * c1.radius, r2_sq = c2.radius * c2.radius,
      l = (d * d + r1_sq - r2_sq) / (2 * d);

    auto v = c2.origin - c1.origin, p = c1.origin + v * l / d;
    if (d == c1.radius + c2.radius) return {p};

    T h = sqrt(r1_sq - l * l);
    auto u = ~v, q = u * h / d;
    return {p + q, p - q};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Circle<double>> circles(n);
    for (auto &[p, r] : circles) cin >> p.x >> p.y >> r;

    vector<bool> removed(n, false);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            if (circles[i].encloses(circles[j])) removed[i] = true;
            if (circles[j].encloses(circles[i])) removed[j] = true;
        }

    vector<int> active;
    for (int i = 0; i < n; i++)
        if (!removed[i]) active.emplace_back(i);
    if (active.empty()) {
        cout << "impossible";
        exit(0);
    }

    for (int i : active) {
        for (int j : active)
            if (i != j && !circles[j].encloses(circles[i])) goto next;
        cout << "voter " << i + 1;
        exit(0);
        next:;
    }

    auto enclosed = [&](auto p) {
        for (int i : active) {
            auto [in, on] = point_in_circle(circles[i], p);
            if (!in && !on) return false;
        }
        return true;
    };

    vector<Point<double>> points;
    for (int l = 0; l < active.size(); l++)
        for (int r = l + 1; r < active.size(); r++)
            for (auto p : circle_circle_intersections(circles[active[l]], circles[active[r]]))
                if (enclosed(p)) points.emplace_back(p);
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end(), [&](auto p1, auto p2) { return approximately_equal(p1.x, p2.x) && approximately_equal(p1.y, p2.y); }), points.end());

    vector<pair<vector<int>, Point<double>>> intersections;
    for (auto p : points) {
        vector<int> voters;
        for (int i : active)
            if (point_in_circle(circles[i], p).second) voters.emplace_back(i);
        if (voters.size() < 2) continue;
        sort(voters.begin(), voters.end());
        intersections.emplace_back(voters, p);
    }
    if (intersections.empty()) {
        cout << "impossible";
        exit(0);
    }

    sort(intersections.begin(), intersections.end(), [&](const auto &i1, const auto &i2) {
        auto &[voters1, p1] = i1;
        auto &[voters2, p2] = i2;
        if (voters1 != voters2) return voters1 < voters2;
        int i = voters1[0], j = voters1[1];
        return (cross(circles[i].origin, circles[j].origin, p1) > 0) > (cross(circles[i].origin, circles[j].origin, p2) > 0);
    });

    cout << "compromise " << intersections.size() << "\n";
    for (auto &[voters, p] : intersections) {
        cout << fixed << setprecision(6) << p.x << " " << p.y << " ";
        for (int i : voters) cout << i + 1 << " ";
        cout << "\n";
    }
}
