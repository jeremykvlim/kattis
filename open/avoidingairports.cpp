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

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull {
    deque<Point<T>> dq;

    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (dq.size() > 1 && violates(dq[1], dq[0], p)) dq.pop_front();
        dq.emplace_front(p);
    }

    T query(T x) {
        auto f = [&](const auto &p) {
            return p.x * x + p.y;
        };

        while (dq.size() > 1 && f(dq[dq.size() - 1]) >= f(dq[dq.size() - 2])) dq.pop_back();
        return f(dq[dq.size() - 1]);
    }

    bool empty() const {
        return dq.empty();
    }

    auto & operator[](int i) {
        return dq[i];
    }

    const auto & operator[](int i) const {
        return dq[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> indices(1e6 + 1, -1);
    vector<array<int, 4>> flights(m);
    for (int i = 0; i < m; i++) {
        auto &[a, b, s, e] = flights[i];
        cin >> a >> b >> s >> e;

        indices[s] = indices[e] = i;
    }

    vector<long long> dp(m + 1, LLONG_MAX);
    vector<MonotonicHull<long long, 1, true>> hull(n + 1);
    hull[1].add(Point(0LL, 0LL));
    auto sum = LLONG_MAX;
    for (int t = 0; t <= 1e6; t++) {
        int i = indices[t];
        if (!~i) continue;

        auto [a, b, s, e] = flights[i];
        if (s == t) {
            if (!hull[a].empty()) dp[i] = hull[a].query(s) + (long long) s * s;
            if (b == n) sum = min(sum, dp[i]);
        }

        if (e == t && dp[i] != LLONG_MAX) hull[b].add(Point(-2LL * e, dp[i] + (long long) e * e));
    }

    cout << sum;
}
