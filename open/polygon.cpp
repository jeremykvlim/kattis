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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
bool simple(const vector<Point<T>> &polygon) {
    int n = polygon.size();

    auto points = polygon;
    points.resize(2 * n);
    points[n] = polygon[n - 1];
    for (int i = n - 1; ~i; i--) {
        if (i) points[i + n] = points[i - 1];
        if (points[i + n] < points[i]) swap(points[i + n], points[i]);
    }

    vector<int> indices(2 * n);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return points[i] != points[j] ? points[i] < points[j] : i > j; });

    auto cmp = [&](int i, int j) {
        T cp = cross(points[j] - points[i], points[j] - points[j + n]);
        return cp < 0 || !sgn(cp) && cross(points[i] - points[i + n], points[j] - points[j + n]) < 0;
    };
    multiset<int, decltype(cmp)> ms(cmp);
    vector<typename decltype(ms)::const_iterator> its(n);
    vector<bool> valid(n, false);
    auto check = [&](int i, int j) {
        T c1 = cross(points[i] - points[i + n], points[i] - points[j + n]);
        if (c1 < 0) return false;

        T c2 = cross(points[j] - points[j + n], points[j] - points[i + n]);
        if (c2 > 0) return false;

        if (c1 || (c2 ? points[i] != points[j] : !sgn(cross(points[i] - points[i + n], points[j] - points[j + n])))) return true;
        return false;
    };
    for (int i : indices)
        if (i < n) {
            auto it = its[i] = ms.emplace(i);
            valid[i] = true;
            if (it != ms.begin() && check(*prev(it), i) || ++it != ms.end() && check(i, *it)) return false;
        } else if (valid[i - n]) {
            auto it = ms.erase(its[i - n]);
            valid[i - n] = false;
            if (it != ms.begin() && it != ms.end() && check(*prev(it), *it)) return false;
        }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<int>> polygon(n);
        for (auto &[x, y] : polygon) cin >> x >> y;
        cout << (simple(polygon) ? "YES\n" : "NO\n");
    }
}
