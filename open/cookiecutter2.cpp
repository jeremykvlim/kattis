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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Point<int>> points(m);
    for (auto &[x, y] : points) cin >> x >> y;

    auto diff = 0.;
    for (int _ = 0; _ < 4; _++) {
        for (int i = 0; i < m; i++) {
            if (2 * points[i].x <= n && 2 * points[i].y <= n) {
                int a = 2 * points[i].x * points[i].y, corner = 0, equal = 0, cut = 0;
                for (int j = 0; j < m; j++) {
                    int lhs = points[i].x * points[j].y + points[i].y * points[j].x;
                    if (lhs < a) corner++;
                    else if (lhs == a) equal++;
                    else cut++;
                }
                double b1 = corner + equal, b2 = cut + equal, chips1 = b1 / m, chips2 = b2 / m, cookie = (double) a / (n * n);
                diff = max({diff, chips1 - cookie, chips2 - (1 - cookie)});
            }
        }

        auto rotate = [&](auto &p) -> Point<int> {
            return {p.y, n - p.x};
        };
        for (int i = 0; i < m; i++) points[i] = rotate(points[i]);
    }

    if (m == 1) {
        cout << fixed << setprecision(6) << diff;
        exit(0);
    }

    for (int i = 0; i < m; i++) {
        int k = m - 1;
        vector<pair<Point<int>, int>> v;
        for (int j = 0; j < m; j++)
            if (i != j) v.emplace_back(points[j] - points[i], j);

        sort(v.begin(), v.end(), [&](auto p1, auto p2) {
            auto quadrant = [](const auto &p) -> int {
                if (p.x > 0) return 1;
                if (!p.x && p.y > 0) return 2;
                if (p.x < 0) return 3;
                if (!p.x && p.y < 0) return 4;
                return 0;
            };

            auto a = p1.first, b = p2.first;
            int qa = quadrant(a), qb = quadrant(b);
            return qa != qb ? qa < qb : cross(a, b) > 0;
        });

        auto cookie = [&](auto &a, auto &b) -> double {
            auto u = ~(a - b);

            auto norm = euclidean_dist(u);
            if (!norm) return 0;

            auto x = fabs(u.x) / norm, y = fabs(u.y) / norm, z = dot(a, u) / norm;
            if (u.x < 0) z += x * n;
            if (u.y < 0) z += y * n;
            if (x < y) swap(x, y);

            if (z <= 0.0) return 0;
            if (z >= (x + y) * n) return 1;
            if (z <= y * n) return pow(z, 2) / (2 * x * y * n * n);
            if (z <= x * n) return (2 * z - y * n) / (2 * x * n);
            return 1 - pow((x + y) * n - z, 2) / (2 * x * y * n * n);
        };

        for (int l = 0, r = 0; l < k; l++) {
            r = max(r, l);
            for (; r < l + k && cross(v[l].first, v[r % k].first) >= 0; r++);
            diff = max(diff, (double) (r - l + 1) / m - cookie(points[i], points[v[l].second]));
            for (; l < k - 1 && !cross(v[l].first, v[l + 1].first) && ((v[l].first.x > 0) == (v[l + 1].first.x > 0)); l++);
        }
    }

    cout << fixed << setprecision(6) << diff;
}
