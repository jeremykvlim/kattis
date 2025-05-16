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
        return sgn(x - p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return sgn(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !sgn(x - p.x) && !sgn(y - p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return sgn(x - p.x) || sgn(y - p.y);
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
void add(deque<Point<T>> &half_hull, Point<T> p, bool collinear = false) {
    auto clockwise = [&]() {
        T cross_product = cross(half_hull[1], half_hull[0], p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    while (half_hull.size() > 1 && clockwise()) half_hull.pop_front();
    half_hull.emplace_front(p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges(m);
    vector<tuple<long long, int, bool>> times(2 * m);
    for (int i = 0; i < m; i++) {
        int a, b, s, e;
        cin >> a >> b >> s >> e;

        edges[i] = {a, b};
        times[2 * i] = {s, i, false};
        times[2 * i + 1] = {e, i, true};
    }
    sort(times.begin(), times.end(), [](auto t1, auto t2) { return get<0>(t1) != get<0>(t2) ? get<0>(t1) < get<0>(t2) : get<2>(t1) < get<2>(t2); });

    vector<long long> dp(m, LLONG_MAX);
    vector<deque<Point<long long>>> half_hulls(n + 1);
    half_hulls[1].emplace_back(0, 0);
    auto sum = LLONG_MAX;
    for (auto [t, i, end] : times) {
        auto [u, v] = edges[i];
        if (end) {
            if (dp[i] == LLONG_MAX) continue;

            add(half_hulls[v], {2 * t, dp[i] + t * t});
        } else {
            auto f = [&](auto p) {
                return -p.x * t + p.y;
            };
            while (half_hulls[u].size() > 1 && f(half_hulls[u].back()) >= f(half_hulls[u][half_hulls[u].size() - 2])) half_hulls[u].pop_back();

            if (!half_hulls[u].empty()) dp[i] = min(dp[i], t * t + f(half_hulls[u].back()));
        }

        if (v == n) sum = min(sum, dp[i]);
    }

    cout << sum;
}
