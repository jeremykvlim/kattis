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
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;
    int i;

    Circle() {}
    Circle(const Point<T> &o, const T &r, const int &i) : origin(o), radius(r), i(i) {}

    bool operator<(const Circle &c) {
        return radius != c.radius ? radius < c.radius : origin < c.origin;
    }

    bool encloses(const Circle &c) const {
        return radius > c.radius && squared_dist(origin, c.origin) < radius * radius;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Circle<int>> circles(n);
    vector<array<int, 2>> energy(n + 1);
    for (int i = 1; i <= n; i++) {
        int x, y, r, a, b;
        cin >> x >> y >> r >> a >> b;

        circles[i - 1] = {{x, y}, r, i};
        energy[i] = {a, b};
    }
    sort(circles.begin(), circles.end());

    int e = 0;
    vector<int> parent(n + 1, 0), depth(n + 1, 0), total(n + 1, 0);
    vector<pair<int, int>> peak(n + 1, {0, 0});
    for (int i = n - 2; ~i; i--)
        for (int j = i + 1; j < n; j++)
            if (circles[j].encloses(circles[i])) {
                int c1 = circles[i].i, c2 = circles[j].i;
                parent[c1] = c2;
                depth[c1] = depth[c2] + 1;
                for (int d = 0;; d++) {
                    if (peak[c1].first < total[c1]) peak[c1] = {total[c1], d};
                    if (d == depth[c1]) break;
                    total[c1] += energy[c1][(d & 1)];
                }
                e += peak[c1].first;
                break;
            }
    cout << e << "\n";

    vector<bool> affected(n + 1, false), drawn(n + 1, false), skip(n + 1, false);
    for (int _ = 0; _ < n; _++) {
        int j = -1;
        for (int i = 0; i < n; i++) {
            int c = circles[i].i;
            affected[c] = false;
            if (skip[c] || depth[c] == peak[c].second && !drawn[c])
                if (parent[c]) skip[parent[c]] = true;
            if (skip[c]) continue;
            if (total[c] == peak[c].first && !drawn[c] && (j < 0 || circles[j].i > c)) j = i;
        }

        int c1 = circles[j].i;
        cout << c1 << " ";
        drawn[c1] = affected[c1] = true;
        for (int i = n - 1; ~i; i--) {
            int c2 = circles[i].i;
            skip[c2] = false;
            if (parent[c2] && affected[parent[c2]]) {
                affected[c2] = true;
                total[c2] -= energy[c2][--depth[c2] & 1];
            }
        }
    }
}