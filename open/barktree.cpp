#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    Point operator+(Point p) const {
        return {x + p.x, y + p.y};
    }

    Point operator-(Point p) const {
        return {x - p.x, y - p.y};
    }

    Point operator*(T c) {
        return {c * x, c * y};
    }
    
    Point operator/(T c) {
        return {x / c, y / c};
    }
};

template <typename T>
double angle(Point<T> p) {
    return atan2(p.y, p.x);
}

template <typename T>
T dot(Point<T> a, Point<T> b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double dist(Point<T> p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

template <typename T>
double dist(Point<T> a, Point<T> b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
int sgn(T v) {
    return fabs(v) < 1e-8 ? 0 : (v > 0) - (v < 0);
}

template <typename T>
pair<bool, bool> point_in_polygon(vector<Point<T>> polygon, Point<T> p) {
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
    Line(T &a, T &b, T &c) {
        if (fabs(a) < 1e-8 && fabs(b) > 1e-8) {
            a = {0, c / b};
            b = {1, c / b};
        } else if (fabs(b) < 1e-8) {
            a = {c / a, 0};
            b = {c / a, 1};
        } else if (fabs(c) < 1e-8) {
            a = {0, c / b};
            b = {1, (c - a) / b};
        } else {
            a = {0, c / b};
            b = {c / a, 0};
        }
    }
};

template <typename T>
Point<T> non_parallel_intersection(Line<T> l1, Line<T> l2) {
    return l2.a + (l2.b - l2.a) * cross(l1.b - l1.a, l1.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
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

                    auto theta = acos(dot(a, b) / (dist(a) * dist(b)));
                    if (smallest > theta) {
                        smallest = theta;
                        tree = t;
                    }
                }
            auto curr = tree != Point(0., 0.) ? non_parallel_intersection(Line(prev, tree), Line(spot, toy)) : toy, b = curr - prev;

            auto theta = fmod(angle(a) - angle(b) + 3 * M_PI, 2 * M_PI) - M_PI;
            if (wrapped.size() > 1 && ((angles.back() < 0) ^ (angles.back() < -theta))) {
                spot = non_parallel_intersection(Line(wrapped[wrapped.size() - 2], prev), Line(spot, toy));
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
            for (int i = 0; i < wrapped.size(); i++) l += dist(wrapped[i], (i + 1 < wrapped.size()) ? wrapped[i + 1] : spot);
            len = max(len, l);
        }

    cout << fixed << setprecision(2) << len;
}
