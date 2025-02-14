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
double dist(const Point<T> &p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

template <typename T>
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
Line<T> perpendicular_bisector(const Point<T> &a, const Point<T> &b) {
    Point<T> midpoint{a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2}, dir{a.y - b.y, b.x - a.x};
    return {midpoint, dir + midpoint};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n;
    double s, t;
    cin >> k >> n >> s >> t;

    vector<Point<double>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;

    if (s >= t) {
        sort(coords.begin(), coords.end(), [&](auto p1, auto p2) {return dist(p1) < dist(p2);});
        cout << fixed << setprecision(6) << t * dist(coords[k - 1]);
        exit(0);
    }

    double kroner = 1e20;
    if (k == 1) {
        for (int i = 0; i < n; i++) kroner = min(kroner, s * dist(coords[i]));
        cout << fixed << setprecision(6) << kroner;
        exit(0);
    }

    shuffle(coords.begin(), coords.end(), mt19937(random_device()()));
    for (int i = 0; i < n; i++) {
        auto check = [&](auto curr) {
            vector<pair<double, int>> angles;
            for (int j = 0; j < n; j++) {
                if (i == j) continue;

                auto [a, b] = perpendicular_bisector(coords[i], coords[j]);
                auto cost = [&](auto m) {
                    auto p = a + (b - a) * m;
                    return s * dist(p) + t * dist(p - coords[i]);
                };

                double l = -1e10, r = 1e10, mid1, mid2;
                while (l + 1e-10 < r && l + l * 1e-10 < r) {
                    mid1 = l + (r - l) / 3, mid2 = r - (r - l) / 3;

                    if (cost(mid1) < cost(mid2)) r = mid2;
                    else l = mid1;
                }
                if (cost(l) > curr) continue;

                double ll = -1e10, lr = l, lm;
                while (ll + 1e-6 < lr && ll + ll * 1e-6 < lr) {
                    lm = ll + (lr - ll) / 2;

                    if (cost(lm) < curr) lr = lm;
                    else ll = lm;
                }

                double rl = l, rr = 1e10, rm;
                while (rl + 1e-10 < rr && rl + rl * 1e-10 < rr) {
                    rm = rl + (rr - rl) / 2;

                    if (cost(rm) < curr) rl = rm;
                    else rr = rm;
                }

                auto a1 = angle(a + (b - a) * ll - coords[i]) + M_PI, a2 = angle(a + (b - a) * rl - coords[i]) + M_PI;
                if (a1 > a2) swap(a1, a2);
                if (a1 + M_PI < a2) {
                    angles.emplace_back(a2, 1);
                    angles.emplace_back(a1 + 2 * M_PI, -1);
                } else {
                    angles.emplace_back(a1, 1);
                    angles.emplace_back(a2, -1);
                    angles.emplace_back(a1 + 2 * M_PI, 1);
                    angles.emplace_back(a2 + 2 * M_PI, -1);
                }
            }
            sort(angles.begin(), angles.end());
            int stars = 1;
            for (auto [angle, c] : angles) {
                stars += c;
                if (stars == k) return true;
            }
            return false;
        };
        if (!check(kroner)) continue;

        double l = 0, r = kroner, mid;
        while (l + 1e-10 < r && l + l * 1e-10 < r) {
            mid = l + (r - l) / 2;

            if (check(mid)) r = mid;
            else l = mid;
        }
        kroner = min(kroner, r);
    }
    cout << fixed << setprecision(6) << kroner;
}
