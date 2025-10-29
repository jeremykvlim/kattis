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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<vector<double>> dist(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j) dist[i][j] = euclidean_dist(points[i], points[j]);
    
    vector<vector<double>> dp(n, vector<double>(n, 0));
    auto dnc = [&](auto &&self, int l, int r, int ql, int qr, int u, bool b) -> void {
        int m = l + (r - l) / 2, qm = 0;
        for (int i = ql; i <= qr && i < m - b; i++) {
            auto d = dist[u][i % n] + dist[i % n][m % n];
            if (dp[u][m % n] < d) {
                dp[u][m % n] = d;
                qm = i;
            }
        }

        if (l < m) self(self, l, m - 1, ql, qm, u, b);
        if (r > m) self(self, m + 1, r, qm, qr, u, b);
    };
    for (int i = 0; i < n; i++) dnc(dnc, i + 2, i + n - 1, i + 1, i + n - 2, i, false);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            dist[i][j] = dp[i][j];
            dp[i][j] = 0;
        }

    for (int i = 0; i < n; i++) dnc(dnc, i + 4, i + n - 1, i + 2, i + n - 3, i, true);

    for (int i = 0; i < n; i++) {
        double d = 0;
        for (int j = i + 4; j <= i + n - 2; j++) d = max(d, dist[j % n][i] + dp[i][j % n]);
        cout << fixed << setprecision(3) << d << "\n";
    }
}
