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
        T cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
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

    vector<Point<long long>> alexa(n);
    for (auto &[x, y] : alexa) cin >> x >> y;
    if (cross(alexa[0], alexa[1], alexa[2]) < 0) reverse(alexa.begin(), alexa.end());

    int k;
    cin >> k;

    vector<Point<long long>> celery(k);
    for (auto &[x, y] : celery) cin >> x >> y;

    auto convex_hull = monotone_chain(celery, true);
    int s = convex_hull.size();
    vector<int> step(n), next(n);
    for (int l = 0, r = 1, d = 0; l < n; l++) {
        for (; cross(alexa[l], convex_hull[d], convex_hull[(d + 1) % s]) < 0 || cross(alexa[l], convex_hull[d], convex_hull[(d + s - 1) % s]) < 0; ++d %= s);
        for (; cross(alexa[l], convex_hull[d], alexa[r]) < 0; ++r %= n);
        step[l] = (r - l - 1 + n) % n;
        next[l] = (r - 1 + n) % n;
    }

    int blocks = ceil(sqrt(n));
    vector<int> dist(n, 0), jump(n);
    for (int i = 0; i < n; i++) {
        int j = i;
        for (int _ = 0; _ < blocks; _++) {
            dist[i] += step[j];
            j = next[j];
        }
        jump[i] = j;
    }

    int count = n;
    for (int i = 0; i < n; i++) {
        int j = i, remaining = n - 1, c = 1;
        while (remaining >= dist[j]) {
            remaining -= dist[j];
            j = jump[j];
            c += blocks;
        }
        while (remaining >= step[j]) {
            remaining -= step[j];
            j = next[j];
            c++;
        }
        count = min(count, c);
    }
    cout << count;
}
