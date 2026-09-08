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

    int n, k, d;
    cin >> n >> k >> d;

    vector<Point<long long>> points(n);
    for (auto &[x, y] : points) {
        double X, Y;
        cin >> X >> Y;

        x = llround(X * 100);
        y = llround(Y * 100);
    }

    auto len = M_PI * d;
    if (k == 1) {
        cout << fixed << setprecision(6) << len;
        exit(0);
    }

    vector<long double> pr(n);
    pr[k - 2] = 1;
    for (int i = 1; i <= k; i++) pr[k - 2] *= (long double) i / (n - k + i);
    for (int i = k - 1; i < n; i++) pr[i] = pr[i - 1] * i / (i - k + 2);

    auto halfplane = [](const auto &p) {
        return (p.y > 0 || (!p.y && p.x > 0)) ? 0 : 1;
    };

    for (int i = 0; i < n; i++) {
        vector<Point<long long>> v;
        for (int j = 0; j < n; j++)
            if (i != j) v.emplace_back(points[j] - points[i]);

        sort(v.begin(), v.end(), [&](auto a, auto b) {
            int ha = halfplane(a), hb = halfplane(b);
            if (ha != hb) return ha < hb;
            if (auto c = cross(a, b)) return c > 0;
            return dot(a, a) < dot(b, b);
        });

        for (int l = 0, r1 = 1, r2 = 1; l < n - 1; l = r1++) {
            for (; r1 < n - 1 && halfplane(v[l]) == halfplane(v[r1]) && !cross(v[l], v[r1]); r1++);
            for (r2 = max(r2, r1); r2 < l + n - 1 && cross(v[l], v[r2 % (n - 1)]) > 0; r2++);

            for (int j = l; j < r1; j++) {
                int valid = r2 - r1 + j - l;
                if (valid >= k - 2) len += euclidean_dist(v[j]) / 100 * pr[valid];
            }
        }
    }
    cout << fixed << setprecision(6) << len;
}
