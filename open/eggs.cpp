#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
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
        return !approximately_equal(x, p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y);
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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, v;
    while (cin >> n >> v && n && v) {
        vector<Point<int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;
        points.emplace_back(0, 0);

        vector<double> sun(n);
        vector<vector<double>> time(n, vector<double>(n + 1));
        for (int i = 0; i < n; i++) {
            sun[i] = (720 + (double) points[i].x / 2000) * 60;

            for (int j = 0; j <= n; j++) time[i][j] = euclidean_dist(points[i], points[j]) / v;
        }

        vector<double> dp(1 << n, INT_MAX);
        dp[0] = 0;
        int eggs = 0;
        for (int m1 = 0; m1 < 1 << n; m1++) {
            for (int i = 0; i < n; i++) {
                if ((m1 >> i) & 1) continue;

                auto ti = time[i][n];
                if (dp[m1] + 2 * ti < sun[i]) dp[m1 | (1 << i)] = min(dp[m1 | (1 << i)], dp[m1] + 3 * ti);
                for (int j = 0; j < i; j++) {
                    if ((m1 >> j) & 1) continue;

                    int m2 = (1 << i) | (1 << j);
                    auto tj = time[j][n];
                    if (dp[m1] + 4 * ti < sun[i] && dp[m1] + 4 * ti + time[i][j] * 2 < sun[j])
                        dp[m1 | m2] = min(dp[m1 | m2], dp[m1] + (4 * ti + tj + time[i][j] * 2));

                    if (dp[m1] + 4 * tj < sun[j] && dp[m1] + 4 * tj + time[j][i] * 2 < sun[i])
                        dp[m1 | m2] = min(dp[m1 | m2], dp[m1] + (4 * tj + ti + time[j][i] * 2));
                }
            }

            if (dp[m1] != INT_MAX) eggs = max(eggs, popcount((unsigned) m1));
        }

        cout << eggs << "\n";
    }
}
