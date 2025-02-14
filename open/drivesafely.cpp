#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-5;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

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

    struct Hash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
double dist(const Point<T> &a, const Point<T> &b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<double> len(n - 1, 0), angle(n, 0);
    angle[0] = 180;
    for (int i = 0; i < n - 1; i++) {
        len[i] = dist(points[i], points[i + 1]);

        if (i < n - 2) {
            auto x1 = points[i + 1].x - points[i].x, y1 = points[i + 1].y - points[i].y,
                 x2 = points[i + 2].x - points[i + 1].x, y2 = points[i + 2].y - points[i + 1].y;

            angle[i + 1] = 180 - abs(atan2(x1 * y2 - x2 * y1, x1 * x2 + y1 * y2)) / M_PI * 180;
        }
    }

    vector<vector<vector<double>>> dp(n - 1, vector<vector<double>>(k + 1, vector<double>(2, DBL_MAX)));
    dp[0][0][0] = 0;
    dp[0][0][1] = len[0] / 180;
    for (int i = 1; i < n - 1 && angle[i] >= 180 - 1e-9; i++) {
        dp[i][0][0] = dp[i - 1][0][1];
        dp[i][0][1] = dp[i][0][0] + len[i] / 180;
    }

    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j <= k; j++) {
            double speed = 180, dist = 0;
            for (int h = i; h; h--) {
                speed = min(speed, angle[h]);
                dist += len[h];

                dp[i][j][1] = min(dp[i][j][1], min(dp[h - 1][j - 1][1] + dist / speed, dp[h - 1][j - 1][0] + (dist + len[h - 1]) / speed));
                dp[i][j][0] = min(dp[i][j][0], min(dp[h - 1][j - 1][1] + (dist - len[i]) / speed, dp[h - 1][j - 1][0] + (dist - len[i] + len[h - 1]) / speed));
            }

            dp[i][j][1] = min(dp[i][j][1], dp[i][j - 1][0] + len[i] / 180);
        }
    }

    auto time = DBL_MAX;
    for (int i = 0; i <= k; i++) time = min(time, dp[n - 2][i][1]);
    cout << fixed << setprecision(6) << time;
}
