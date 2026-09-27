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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
pair<array<int, 3>, T> closest_triple(const vector<Point<T>> &points) {
    int n = points.size();

    vector<pair<Point<T>, int>> sorted(n);
    for (int i = 0; i < n; i++) sorted[i] = {points[i], i};
    sort(sorted.begin(), sorted.end(), [](auto p1, auto p2) { return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first; });

    T d = numeric_limits<T>::max();
    int a = -1, b = -1, c = -1;
    auto update = [&](auto p1, auto p2, auto p3) {
        if (!cross(p1.first, p2.first, p3.first)) return false;

        auto perimeter = euclidean_dist(p1.first, p2.first) + euclidean_dist(p1.first, p3.first) + euclidean_dist(p2.first, p3.first);
        if (d > perimeter) {
            d = perimeter;
            a = p1.second;
            b = p2.second;
            c = p3.second;
        }
        return true;
    };

    auto sq = [](T v) -> T { return v * v; };
    auto cmp = [](auto p1, auto p2) { return p1.first.y < p2.first.y; };
    multiset<pair<Point<T>, int>, decltype(cmp)> ms(cmp);
    vector<typename decltype(ms)::const_iterator> its(n);
    for (int i = 0, j = 0; i < n; i++) {
        for (; j < i && 4 * sq(sorted[j].first.x - sorted[i].first.x) >= sq(d); j++) ms.erase(its[j]);

        vector<pair<Point<T>, int>> candidates;
        auto it = ms.upper_bound(sorted[i]);
        if (it != ms.begin()) {
            candidates.emplace_back(*prev(it));
            for (auto p = prev(it); p != ms.begin() && 4 * sq(sorted[i].first.y - p->first.y) < sq(d);) candidates.emplace_back(*(--p));
        }
        for (; it != ms.end() && 4 * sq(it->first.y - sorted[i].first.y) < sq(d); it++) candidates.emplace_back(*it);
        for (int x = 0; x < candidates.size(); x++)
            for (int y = x + 1; y < candidates.size(); y++) update(candidates[x], candidates[y], sorted[i]);
        its[i] = ms.emplace_hint(ms.upper_bound(sorted[i]), sorted[i]);
    }

    return {{a, b, c}, d};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;
    cout << fixed << setprecision(6) << closest_triple(points).second;
}
