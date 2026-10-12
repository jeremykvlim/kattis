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

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull : deque<Point<T>> {
    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (this->size() > 1 && violates((*this)[1], (*this)[0], p)) this->pop_front();
        this->emplace_front(p);
    }

    T query(T x) {
        auto f = [&](const auto &p) {
            return p.x * x + p.y;
        };

        while (this->size() > 1 && f(this->back()) >= f((*this)[this->size() - 2])) this->pop_back();
        return f(this->back());
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> a(n, vector<int>(n));
    vector<vector<pair<int, int>>> rows(k + 1), cols(k + 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];

            rows[a[i][j]].emplace_back(i, j);
        }

    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++) cols[a[i][j]].emplace_back(i, j);

    for (int v = 1; v <= k; v++)
        if (rows[v].empty()) {
            cout << -1;
            exit(0);
        }

    MonotonicHull<long long, 1, true> hull;
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    for (int v = 2; v <= k; v++) {
        hull.clear();
        for (auto [i, j] : rows[v - 1]) hull.add(Point<long long>{-2 * i, dp[i][j] + (long long) i * i});
        for (auto [i, j] : rows[v]) dp[i][j] = (long long) i * i + hull.query(i);
        hull.clear();
        for (auto [i, j] : cols[v - 1]) hull.add(Point<long long>{-2 * j, dp[i][j] + (long long) j * j});
        for (auto [i, j] : cols[v]) dp[i][j] = min(dp[i][j], hull.query(j) + (long long) j * j);
    }

    auto len = LLONG_MAX;
    for (auto [i, j] : rows[k]) len = min(len, dp[i][j]);
    cout << len;
}
