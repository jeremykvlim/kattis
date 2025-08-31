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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
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
vector<pair<Point<T>, int>> monotone_chain(vector<pair<Point<T>, int>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<pair<Point<T>, int>> convex_hull;

    auto clockwise = [&](auto p) {
        T cross_product = cross(convex_hull[convex_hull.size() - 2].first, convex_hull.back().first, p.first);
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int &ai : a) cin >> ai;
    for (int &bi : b) cin >> bi;

    int tx, ty;
    cin >> tx >> ty;

    if (n == 1) {
        if (a[0] == tx && b[0] == ty) cout << "Yes\n1\n";
        else cout << "No";
        exit(0);
    }

    vector<pair<Point<int>, int>> points(n);
    for (int i = 0; i < n; i++) points[i] = {{a[i], b[i]}, i};
    auto convex_hull = monotone_chain(points, true);

    int m = convex_hull.size();
    if (m == 2 && convex_hull[0].first == convex_hull[1].first) {
        convex_hull.pop_back();
        m--;
    }

    vector<double> p(n, 0);
    pair<Point<int>, int> target{{tx, ty}, -1};
    if (m == 1) {
        if (convex_hull[0].first == target.first) {
            p[convex_hull[0].second] = 1;
            cout << "Yes\n";
            for (auto pi : p) cout << fixed << setprecision(8) << pi << " ";
        } else cout << "No";
        exit(0);
    }

    if (m == 2) {
        auto A = convex_hull[0], B = convex_hull[1];
        if (cross(A.first, B.first, target.first) || dot(target.first - A.first, target.first - B.first) > 0) {
            cout << "No";
            exit(0);
        }

        auto q = (A.first.x != B.first.x) ? (double) (target.first.x - A.first.x) / (B.first.x - A.first.x) :
                 (double) (target.first.y - A.first.y) / (B.first.y - A.first.y);
        p[A.second] = 1. - q;
        p[B.second] = q;
        cout << "Yes\n";
        for (auto pi : p) cout << fixed << setprecision(8) << pi << " ";
        exit(0);
    }

    int j = -1;
    for (int i = 1; i + 1 < m; i++) {
        vector<Point<int>> triangle{convex_hull[0].first, convex_hull[i].first, convex_hull[i + 1].first};
        auto [in, on] = point_in_polygon(triangle, target.first);
        if (in || on) {
            j = i;
            break;
        }
    }
    if (!~j) {
        cout << "No";
        exit(0);
    }

    auto A = convex_hull[0], B = convex_hull[j], C = convex_hull[j + 1];
    int c = cross(A.first, B.first, C.first);
    p[A.second] = (double) cross(target.first, B.first, C.first) / c;
    p[B.second] = (double) cross(target.first, C.first, A.first) / c;
    p[C.second] = (double) cross(target.first, A.first, B.first) / c;
    cout << "Yes\n";
    for (auto pi : p) cout << fixed << setprecision(8) << pi << " ";
}
