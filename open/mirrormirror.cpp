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

template <typename T>
Point<T> midpoint(const Point<T> &a, const Point<T> &b) {
    return {a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2};
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
Line<T> perpendicular_bisector(const Point<T> &a, const Point<T> &b) {
    Point<T> mp = midpoint(a, b), dir{a.y - b.y, b.x - a.x};
    return {mp, dir + mp};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<vector<int>> count(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        vector<pair<int, int>> v;
        for (int j = 0; j < n; j++)
            if (i != j) v.emplace_back(squared_dist(points[i], points[j]), j);
        sort(v.begin(), v.end());

        for (int l = 0, r = 0; r < n - 1; r++)
            if (r == n - 2 || v[r].first != v[r + 1].first) {
                if (l < r)
                    for (int j = l; j < r; j++)
                        for (int k = j + 1; k <= r; k++) count[v[j].second][v[k].second]++;
                l = r + 1;
            }
    }

    vector<array<int, 3>> coeffs1, coeffs2;
    vector<pair<int, int>> indices;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            auto add = [&](const auto &l, const auto &p, auto &coeffs) {
                auto dir = l.b - l.a;
                int g = gcd(abs(dir.x), abs(dir.y));
                dir /= g;
                int a = dir.y, b = dir.x;
                if (a < 0 || (!a && b < 0)) {
                    a = -a;
                    b = -b;
                }
                coeffs.push_back({a, b, a * p.x - b * p.y});
            };
            add(Line{points[i], points[j]}, points[i], coeffs1);
            add(perpendicular_bisector(points[i], points[j]), points[i] + points[j], coeffs2);

            indices.emplace_back(i, j);
        }

    int size = 1;
    sort(coeffs1.begin(), coeffs1.end());
    for (int l = 0, r = 0; r < coeffs1.size(); r++)
        if (r == coeffs1.size() - 1 || coeffs1[r] != coeffs1[r + 1]) {
            int s = 0;
            for (; s * (s + 1) / 2 <= r - l + 1; s++);
            size = max(size, s);
            l = r + 1;
        }

    vector<int> order(coeffs2.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return coeffs2[i] < coeffs2[j]; });
    for (int l = 0, r = 0; r < coeffs2.size(); r++)
        if (r == coeffs2.size() - 1 || coeffs2[order[r]] != coeffs2[order[r + 1]]) {
            auto [i, j] = indices[order[r]];
            size = max(size, 2 * (r - l + 1) + count[i][j]);
            l = r + 1;
        }
    cout << size;
}
