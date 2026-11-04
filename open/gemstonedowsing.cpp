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
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<Point<double>> locations(n + 1, {0, 0});
    while (k--) {
        int x, y, f;
        cin >> x >> y >> f;

        locations[f] = {x, y};
    }

    vector<int> order(n);
    vector<Point<double>> readings(n + 1, {0, 0});
    for (int &f : order) {
        int x;
        cin >> x >> f;

        readings[f] = {x, 0};
    }

    vector<int> left(n);
    for (int i = 0, l = 0; i < n; i++) {
        left[i] = l;
        if (locations[order[i]].y) l = order[i];
    }

    for (int i = n - 1, r = 0; ~i; i--) {
        int f = order[i];
        if (locations[f].y) {
            r = f;
            continue;
        }

        int l = left[i];
        auto dl = l ? squared_dist(locations[l], readings[f]) : 1e20, dr = r ? squared_dist(locations[r], readings[f]) : 1e20, d = min(dl, dr), x = readings[f].x;
        if (l) x = max(x, locations[l].x + (dl - d) / (2 * (readings[f].x - readings[l].x)));
        if (r) x = min(x, locations[r].x + (dr - d) / (2 * (readings[f].x - readings[r].x)));
        readings[f].y = -sqrt(max(0., d - (x - readings[f].x) * (x - readings[f].x)));
    }

    for (int f = 1; f <= n; f++)
        if (!locations[f].y) cout << fixed << setprecision(5) << f << " " << readings[f].y << "\n";
}