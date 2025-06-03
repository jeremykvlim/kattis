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
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == -1 : sgn(y - p.y) == -1;
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == 1 : sgn(y - p.y) == 1;
        return x != p.x ? x > p.x : y > p.y;
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<long long>> points(n);
    long long xl = 1e18, xr = -1e18, yl = 1e18, yr = -1e18;
    for (auto &[x, y] : points) {
        cin >> x >> y;

        xl = min(xl, x);
        xr = max(xr, x);
        yl = min(yl, y);
        yr = max(yr, y);
    }

    auto width = xr - xl, height = yr - yl, square_len = max(width, height);
    vector<array<long long, 4>> candidates;
    if (width == height) candidates.push_back({xl, xr, yl, yr});
    else if (width > height) {
        candidates.push_back({xl, xr, yl, yl + square_len});
        candidates.push_back({xl, xr, yr - square_len, yr});
    } else {
        candidates.push_back({xl, xl + square_len, yl, yr});
        candidates.push_back({xr - square_len, xr, yl, yr});
    }

    int s = candidates.size();
    vector<bool> possible(s, true);
    for (auto [x, y] : points) {
        for (int i = 0; i < s; i++) {
            if (!possible[i]) continue;
            auto [x1, x2, y1, y2] = candidates[i];
            if (!(x == x1 || x == x2 || y == y1 || y == y2)) possible[i] = false;
        }

        if (all_of(possible.begin(), possible.end(), [&](bool b) { return !b; })) {
            cout << "Impossible";
            exit(0);
        }
    }

    for (int i = 0; i < s; i++)
        if (possible[i]) {
            auto [x1, x2, y1, y2] = candidates[i];
            cout << x1 << " " << x2 << " " << y1 << " " << y2;
            exit(0);
        }
}
