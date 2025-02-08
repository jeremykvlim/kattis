#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-6;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

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

    struct Hash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
double dist(const Point<T> &a, const Point<T> &b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        auto cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
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

    vector<Point<long long>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<double> d(1 << n, 0), dp(1 << n, 1e18);
    dp[0] = 0;
    for (int i = 0; i < 1 << n; i++) {
        vector<Point<long long>> enclosure;
        for (int j = 0; j < n; j++)
            if ((i >> j) & 1) enclosure.emplace_back(points[j]);
        if (enclosure.empty()) continue;

        auto convex_hull = monotone_chain(enclosure);
        for (int j = convex_hull.size() - 1, k = 0; k < convex_hull.size(); j = k++) d[i] += dist(convex_hull[j], convex_hull[k]);
    }

    for (int i = 1; i < 1 << n; i++)
        for (int j = i; j; --j &= i)
            if (__builtin_popcount(j) >= 3) dp[i] = min(dp[i], dp[i ^ j] + d[j]);

    cout << fixed << setprecision(6) << dp.back();
}
