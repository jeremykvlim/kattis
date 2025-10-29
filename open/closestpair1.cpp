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
pair<pair<int, int>, T> closest_pair(const vector<Point<T>> &points) {
    int n = points.size();

    vector<pair<Point<T>, int>> sorted(n);
    for (int i = 0; i < n; i++) sorted[i] = {points[i], i};
    sort(sorted.begin(), sorted.end(), [](auto p1, auto p2) { return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first; });

    T d = numeric_limits<T>::max();
    int a = -1, b = -1;
    auto update = [&](auto p1, auto p2) {
        auto dist = squared_dist(p1.first, p2.first);
        if (d > dist) {
            d = dist;
            a = p1.second;
            b = p2.second;
        }
    };

    auto sq = [](T v) -> T { return v * v; };
    auto cmp = [](auto p1, auto p2) { return p1.first.y < p2.first.y; };
    multiset<pair<Point<T>, int>, decltype(cmp)> ms(cmp);
    vector<typename decltype(ms)::const_iterator> its(n);
    for (int i = 0, j = 0; i < n; i++) {
        for (; j < i && sq(sorted[j].first.x - sorted[i].first.x) >= d; j++) ms.erase(its[j]);

        auto it = ms.upper_bound(sorted[i]);
        if (it != ms.begin()) {
            update(*prev(it), sorted[i]);
            for (auto p = prev(it); p != ms.begin() && sq(sorted[i].first.y - p->first.y) < d;) update(*(--p), sorted[i]);
        }
        for (; it != ms.end() && sq(it->first.y - sorted[i].first.y) < d; it++) update(*it, sorted[i]);
        its[i] = ms.emplace_hint(ms.upper_bound(sorted[i]), sorted[i]);
    }

    return {{a, b}, d};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<double>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;
        auto [pair, d] = closest_pair(points);
        auto [a, b] = pair;
        cout << fixed << setprecision(2) << points[a].x << " " << points[a].y << " " << points[b].x << " " << points[b].y << "\n";
    }
}
