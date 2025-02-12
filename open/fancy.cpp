#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-6;
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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    k = min(m, k);

    vector<Point<long long>> onions(n), polygon(m);
    for (auto &[x, y] : onions) cin >> x >> y;
    for (auto &[x, y] : polygon) cin >> x >> y;
    reverse(polygon.begin(), polygon.end());

    vector<vector<int>> outside(m, vector<int>(m));
    for (int i = 0; i < m; i++) {
        sort(onions.begin(), onions.end(), [&](auto p1, auto p2) {return cross(p1, p2, polygon[i]) > 0;});
        for (int j = 1, count = 0; j < m; j++) {
            while (count < n && cross(onions[count], polygon[i], polygon[(i + j) % m]) <= 0) count++;
            outside[(i + j) % m][i] = count;
        }
    }

    vector<vector<vector<int>>> memo(m, vector<vector<int>>(m, vector<int>(k + 1, -1)));
    auto dp = [&](auto &&self, int l, int r, int upgrades) {
        if (!~upgrades || r - l > m) return n;
        if (r - l == m) return 0;

        if (~memo[l % m][r % m][upgrades]) return memo[l % m][r % m][upgrades];
        int stolen = n;
        for (int i = r + 1; i < r + m; i++) stolen = min(stolen, outside[i % m][r % m] + self(self, l, i, upgrades - 1));
        return memo[l % m][r % m][upgrades] = stolen;
    };

    int stolen = n;
    for (int i = 0; i < m / k; i++) stolen = min(stolen, dp(dp, i, i, k));
    cout << n - stolen;
}
