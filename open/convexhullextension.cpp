#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
tuple<T, T, T> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        auto q = x / y, r = x - q * y;
        return {q, r};
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<int>> convex_hull(n);
    for (auto &[x, y] : convex_hull) cin >> x >> y;

    auto count = [&](const Point<int> &u, const Point<int> &v, const Point<int> &a, const Point<int> &b) -> long long {
        auto w = u + v;
        auto [g, s, t] = extended_gcd(w.x, w.y);
        w /= g;

        Point<int> p = b - a, q = a, r{-t, s};
        if (cross(w, p) < 0) w = -w;
        q += w;
        if (cross(w, r) < 0) r = -r;
        do r += w; while (cross(r - w, p) < 0);

        auto c = 0LL;
        for (;;) {
            q -= w * ((cross(q - a, p) - 1) / cross(w, p) + 1);
            if (q == b) return c;
            c += max(0, min((cross(q - a, u) + 1) / cross(u, w), (cross(q - b, v) - 1) / cross(v, w)));
            q += r;
        }
    };

    auto points = 0LL;
    for (int i = 0; i < n; i++) {
        auto v1 = convex_hull[(i + 1) % n] - convex_hull[i], v2 = convex_hull[(i + 3) % n] - convex_hull[(i + 2) % n];
        auto cp = cross(v1, v2);
        if (cp < 0 || !cp && abs(cross(v1, convex_hull[(i + 3) % n] - convex_hull[i])) != abs(get<0>(extended_gcd(v1.x, v1.y)))) {
            cout << "infinitely many";
            exit(0);
        }
        if (cp) points += count(v1, -v2, convex_hull[(i + 1) % n], convex_hull[(i + 2) % n]);
    }
    cout << points;
}
