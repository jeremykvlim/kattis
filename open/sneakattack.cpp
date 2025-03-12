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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Point<double> m1, m2, p;
    while (cin >> m1.x >> m1.y >> m2.x >> m2.y >> p.x >> p.y) {
        auto dir = m2 - m1, unit = dir / euclidean_dist(m1, m2), v1 = p - m1, v2 = m1 - v1;
        auto s = v2 + unit * 2 * dot(v1, unit);

        if (approximately_equal(s.x, 0.)) {
            cout << "can't hit the wall\n";
            continue;
        }

        int sgn1 = sgn(s.x);
        auto xl = min(m1.x, m2.x), xr = max(m1.x, m2.x);
        double l, r;
        if (sgn1 == 1) {
            l = max(xl, 0.);
            r = min(xr, s.x);
        } else {
            l = max(xl, s.x);
            r = min(xr, 0.);
        }

        if (l + 1e-8 >= r) {
            cout << "can't hit the wall\n";
            continue;
        }

        if (approximately_equal(dir.x, 0.)) {
            if (sgn1 == 1) {
                if (!(0 <= m1.x && m1.x < s.x)) {
                    cout << "can't hit the wall\n";
                    continue;
                }
            } else {
                if (!(s.x <= m1.x && m1.x < 0)) {
                    cout << "can't hit the wall\n";
                    continue;
                }
            }

            auto y_intercept = [&](double X1, double Y1, double X2, double Y2) -> double {
                return Y1 - X1 * (Y2 - Y1) / (X2 - X1);
            };
            auto yl = min(m1.y, m2.y), yr = max(m1.y, m2.y), y1 = y_intercept(s.x, s.y, m1.x, yl), y2 = y_intercept(s.x, s.y, m1.x, yr);
            cout << fixed << setprecision(4) << min(y1, y2) << " " << max(y1, y2) << "\n";
            continue;
        }

        auto slope = dir.y / dir.x;
        auto y_intercept = [&](double X1, double Y1, double X2, double Y2) -> double {
            return Y1 - X1 * ((Y2 - Y1) + slope * (X2 - m1.x)) / (X2 - X1);
        };

        auto v3 = s - m1;
        auto offset = slope * v3.x - v3.y;
        int sgn2 = sgn(offset);
        double y1, y2;
        if (sgn1 == 1) {
            y1 = y_intercept(s.x, s.y, l, m1.y);
            if (approximately_equal(r, s.x)) y2 = sgn2 * numeric_limits<double>::infinity();
            else y2 = y_intercept(s.x, s.y, r, m1.y);
            if (sgn2 == -1) swap(y1, y2);
        } else {
            y2 = y_intercept(s.x, s.y, r, m1.y);
            if (approximately_equal(l, s.x)) y1 = sgn2 * numeric_limits<double>::infinity();
            else y1 = y_intercept(s.x, s.y, l, m1.y);
            if (sgn2 == 1) swap(y1, y2);
        }

        auto dtos = [&](double v) -> string {
            ostringstream oss;
            oss << fixed << setprecision(4) << v;
            return !isalpha(oss.str().back()) ? oss.str() : (oss.str() == "inf" ? "positive-infinity" : "negative-infinity");
        };
        cout << dtos(y1) << " " << dtos(y2) << "\n";
    }
}
