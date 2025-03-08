#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Fraction {
    T numer, denom;

    Fraction() {}
    Fraction(T n, T d) : numer(n), denom(d) {
        reduce();
    }

    void reduce() {
        if (denom < 0) {
            numer *= -1;
            denom *= -1;
        }

        T g = __gcd(abs(numer), denom);
        if (g) {
            numer /= g;
            denom /= g;
        }
    }

    bool operator<(const Fraction &f) const {
        return numer * f.denom < f.numer * denom;
    }

    bool operator>(const Fraction &f) const {
        return numer * f.denom > f.numer * denom;
    }

    bool operator==(const Fraction &f) const {
        return numer == f.numer && denom == f.denom;
    }

    bool operator!=(const Fraction &f) const {
        return numer != f.numer || denom != f.denom;
    }

    bool operator<=(const Fraction &f) const {
        return *this < f || *this == f;
    }

    bool operator>=(const Fraction &f) const {
        return *this > f || *this == f;
    }

    Fraction operator+(const Fraction &f) const {
        return {numer * f.denom + f.numer * denom, denom * f.denom};
    }

    Fraction operator+(const T &v) const {
        return {numer + v * denom, denom};
    }

    Fraction & operator+=(const Fraction &f) {
        numer = numer * f.denom + f.numer * denom;
        denom *= f.denom;
        reduce();
        return *this;
    }

    Fraction & operator+=(const T &v) {
        numer += v * denom;
        reduce();
        return *this;
    }

    Fraction operator-(const Fraction &f) const {
        return {numer * f.denom - f.numer * denom, denom * f.denom};
    }

    Fraction operator-(const T &v) const {
        return {numer - v * denom, denom};
    }

    Fraction & operator-=(const Fraction &f) {
        numer = numer * f.denom - f.numer * denom;
        denom *= f.denom;
        reduce();
        return *this;
    }

    Fraction & operator-=(const T &v) {
        numer -= v * denom;
        reduce();
        return *this;
    }

    Fraction operator*(const Fraction &f) const {
        return {numer * f.numer, denom * f.denom};
    }

    Fraction operator*(const T &v) const {
        return {numer * v, denom};
    }

    Fraction & operator*=(const Fraction &f) {
        numer *= f.numer;
        denom *= f.denom;
        reduce();
        return *this;
    }

    Fraction & operator*=(const T &v) {
        numer *= v;
        reduce();
        return *this;
    }

    Fraction operator/(const Fraction &f) const {
        return {numer * f.denom, denom * f.numer};
    }

    Fraction operator/(const T &v) const {
        return {numer, denom * v};
    }

    Fraction & operator/=(const Fraction &f) {
        numer *= f.denom;
        denom *= f.numer;
        reduce();
        return *this;
    }

    Fraction & operator/=(const T &v) {
        denom *= v;
        reduce();
        return *this;
    }

    struct FractionHash {
        size_t operator()(Fraction<T> f) const {
            auto h = 0ULL;
            h ^= hash<T>()(f.numer) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(f.denom) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

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

    struct PointHash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
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
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
bool point_on_line(const Point<T> &p, const Line<T> &l) {
    return !sgn(cross(l.b - l.a, p - l.a)) && (dot(l.a - p, l.b - p) < 0 || approximately_equal(dot(l.a - p, l.b - p), (T) 0));
}

template <typename T>
bool parallel(const Line<T> &l1, const Line<T> &l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
bool collinear(const Line<T> &l1, const Line<T> &l2) {
    return parallel(l1, l2) && !sgn(cross(l1.b - l1.a, l2.a - l1.a));
}

template <typename T>
bool intersects(const Line<T> &l1, const Line<T> &l2) {
    return collinear(l1, l2) || !parallel(l1, l2);
}

template <typename T>
vector<Point<T>> collinear_overlap(const Line<T> &l1, const Line<T> &l2) {
    auto p1 = min(l1.a, l1.b), p2 = max(l1.a, l1.b), p3 = min(l2.a, l2.b), p4 = max(l2.a, l2.b);
    if (p3 <= p2 && p1 <= p4) {
        auto l = max(p1, p3), r = min(p2, p4);
        if (l == r) return {l};
        return {l, r};
    }
    return {};
}

template <typename T>
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Line<long double>> lines(n);
    for (auto &[a, b] : lines) cin >> a.x >> a.y >> b.x >> b.y;

    set<pair<Fraction<long long>, Fraction<long long>>> points;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            auto l1 = lines[i], l2 = lines[j];

            if (!intersects(l1, l2)) continue;
            else if (collinear(l1, l2)) {
                auto v = collinear_overlap(l1, l2);
                if (v.empty()) continue;
                else if (v.size() > 1) {
                    cout << -1;
                    exit(0);
                } else points.emplace(Fraction((long long) v[0].x, 1LL), Fraction((long long) v[0].y, 1LL));
            } else {
                auto p = non_collinear_intersection(l1, l2);
                if (point_on_line(p, l1) && point_on_line(p, l2)) {
                    long long numer = cross(l2.a - l1.a, l2.b - l2.a), denom = cross(l1.b - l1.a, l2.b - l2.a);
                    Point<long long> before_div = l1.a * denom + (l1.b - l1.a) * numer;
                    points.emplace(Fraction(before_div.x, denom), Fraction(before_div.y, denom));
                }
            }
        }
    cout << points.size();
}
