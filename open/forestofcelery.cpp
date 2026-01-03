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

template <typename T>
struct JumpTable {
    int lg;
    vector<vector<int>> lift;
    vector<vector<T>> sum;

    JumpTable(int n, int m, const vector<int> &next, const vector<T> &w) {
        lg = __lg(n) + 1;
        lift.assign(lg, vector<int>(m));
        lift[0] = next;
        sum.assign(lg, vector<T>(m));
        sum[0] = w;

        for (int b = 1; b < lg; b++)
            for (int i = 0; i < m; i++) {
                int j = lift[b - 1][i];
                lift[b][i] = lift[b - 1][j];
                sum[b][i] = sum[b - 1][i] + sum[b - 1][j];
            }
    }

    pair<T, int> jump_up(int v, int k) const {
        T cost = 0;
        int steps = 0;
        for (int b = 0; b < lg; b++)
            if ((k >> b) & 1) {
                cost += sum[b][v];
                steps += 1 << b;
                v = lift[b][v];
            }
        return {cost, steps};
    }

    pair<T, int> jump_down(int v, T bound) const {
        T cost = 0;
        int steps = 0;
        for (int b = lg - 1; ~b; b--)
            if (cost + sum[b][v] <= bound) {
                cost += sum[b][v];
                steps += 1 << b;
                v = lift[b][v];
            }
        return {cost, steps};
    }
};

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
    vector<int> next(n);
    vector<long long> w(n);
    for (int l = 0, r = 1, d = 0; l < n; l++) {
        for (; cross(alexa[l], convex_hull[d], convex_hull[(d + 1) % s]) < 0 || cross(alexa[l], convex_hull[d], convex_hull[(d + s - 1) % s]) < 0; ++d %= s);
        for (; cross(alexa[l], convex_hull[d], alexa[r]) < 0; ++r %= n);
        next[l] = (r - 1 + n) % n;
        w[l] = (r - l - 1 + n) % n;
    }

    JumpTable<long long> jt(n, n, next, w);
    int count = n;
    for (int i = 0; i < n; i++) count = min(count, jt.jump_down(i, (long long) n - 1).second + 1);
    cout << count;
}
