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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}

    bool operator==(const Circle &c) const {
        if constexpr (is_floating_point_v<T>) return origin == c.origin && !sgn(radius - c.radius);
        return origin == c.origin && radius == c.radius;
    }
};

template <typename T>
vector<Point<T>> circle_circle_intersections(const Circle<T> &c1, const Circle<T> &c2) {
    if (c1 == c2) return {};

    T d = euclidean_dist(c1.origin, c2.origin);
    if (d > c1.radius + c2.radius || d < abs(c1.radius - c2.radius)) return {};

    T r1_sq = c1.radius * c1.radius, r2_sq = c2.radius * c2.radius,
            l = (d * d + r1_sq - r2_sq) / (2 * d);

    Point<T> v = c2.origin - c1.origin, p = c1.origin + v * l / d;
    if (d == c1.radius + c2.radius) return {p};

    T h = sqrt(r1_sq - l * l);
    Point<T> u = {-v.y, v.x}, q = u * h / d;
    return {p + q, p - q};
}

template <typename T>
bool intersects(const array<Point<T>, 3> &t1, const array<Point<T>, 3> &t2) {
    array<Point<T>, 6> axes;

    for (int i = 0; i < 3; i++) {
        auto v1 = t1[(i + 1) % 3] - t1[i], v2 = t2[(i + 1) % 3] - t2[i];
        axes[i] = {-v1.y, v1.x};
        axes[i + 3] = {-v2.y, v2.x};
    }

    for (auto axis : axes) {
        T l1 = numeric_limits<T>::infinity(), r1 = -numeric_limits<T>::infinity(),
                l2 = numeric_limits<T>::infinity(), r2 = -numeric_limits<T>::infinity();

        for (int i = 0; i < 3; i++) {
            T projection1 = dot(t1[i], axis), projection2 = dot(t2[i], axis);
            l1 = min(l1, projection1);
            r1 = max(r1, projection1);
            l2 = min(l2, projection2);
            r2 = max(r2, projection2);
        }

        if (sgn(r1 - l2) <= 0 || sgn(r2 - l1) <= 0) return false;
    }
    return true;
}

template <typename T>
T heron(T s, T a, T b, T c) {
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<array<long double, 3>> triangles(4);
    while (t--) {
        for (auto &[a, b, c] : triangles) {
            cin >> a >> b >> c;

            a = sqrtl(a);
            b = sqrtl(b);
            c = sqrtl(c);
        }

        auto total = 0.L;
        vector<long double> area(4);
        for (int i = 0; i < 4; i++) {
            sort(triangles[i].begin(), triangles[i].end());
            auto [a, b, c] = triangles[i];
            auto s = (a + b + c) / 2;
            total += area[i] = heron(s, a, b, c);
        }

        if (abs(llround(total) - total) > total * 1e-9) {
            cout << "0\n";
            continue;
        }

        auto len = sqrtl(llround(total));
        vector<int> order{0, 1, 2, 3};
        sort(order.begin(), order.end(), [&](int i, int j) {
            return area[i] != area[j] ? area[i] > area[j] : triangles[i][2] > triangles[j][2];
        });

        vector<vector<array<long double, 3>>> perms(4);
        for (int i = 0; i < 4; i++) {
            auto triangle = triangles[i];
            do perms[i].emplace_back(triangle);
            while (next_permutation(triangle.begin(), triangle.end()));
        }

        vector<array<Point<long double>, 3>> prev;
        vector<Line<long double>> edges{{{0, 0}, {len, 0}}, {{len, 0}, {len, len}}, {{len, len}, {0, len}}, {{0, len}, {0, 0}}};
        auto backtrack = [&](auto &&self, int mask = 0) -> bool {
            if (mask == 15) return true;

            int n = edges.size();
            for (int i = 0; i < n; i++) {
                auto e1 = edges[i], e2 = edges.back();
                swap(edges[i], edges.back());
                edges.pop_back();
                for (int j = 0; j < 2; j++) {
                    auto [a, b] = e1;
                    if (j) swap(a, b);
                    auto v = b - a;

                    for (int k = 0; k < 4; k++)
                        if (!((mask >> k) & 1)) {
                            for (auto p : perms[order[k]]) {
                                auto d = euclidean_dist(a, b);
                                if (sgn(d - p[0]) == 1) continue;

                                auto points = circle_circle_intersections(Circle<long double>{a, p[1]}, Circle<long double>{a + v * p[0] / d, p[2]});
                                if (points.size() < 2) continue;

                                array<Point<long double>, 3> triangle{a, a + v * p[0] / d, points[j]};
                                if (sgn(min({triangle[0].x, triangle[1].x, triangle[2].x, triangle[0].y, triangle[1].y, triangle[2].y})) == -1 ||
                                    sgn(max({triangle[0].x, triangle[1].x, triangle[2].x, triangle[0].y, triangle[1].y, triangle[2].y}) - len) == 1) goto next;

                                for (auto tri : prev)
                                    if (intersects(tri, triangle)) goto next;

                                prev.emplace_back(triangle);
                                if (j) {
                                    edges.emplace_back(triangle[2], triangle[0]);
                                    edges.emplace_back(triangle[1], triangle[2]);
                                } else {
                                    edges.emplace_back(triangle[0], triangle[2]);
                                    edges.emplace_back(triangle[2], triangle[1]);
                                }
                                if (self(self, mask | (1 << k))) return true;
                                edges.pop_back();
                                edges.pop_back();
                                prev.pop_back();
                                next:;
                            }
                        }
                }
                edges.emplace_back(e2);
                edges[i] = e1;
            }
            return false;
        };
        cout << backtrack(backtrack) << "\n";
    }
}
