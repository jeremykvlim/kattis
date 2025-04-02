#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

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
    Line(const T &m, const T &n, const T &c) {
        if (approximately_equal(m, (T) 0) && !approximately_equal(n, (T) 0)) {
            a = {0, c / n};
            b = {1, c / n};
        } else if (!approximately_equal(m, (T) 0) && approximately_equal(n, (T) 0)) {
            a = {c / m, 0};
            b = {c / m, 1};
        } else {
            a = {0, c / n};
            b = {1, (c - m) / n};
        }
    }
};

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

template <typename T>
tuple<T, T, T> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        auto div = [&](const T &x, const T &y) {
            auto numer = x * conj(y);
            auto denom = norm(y);
            auto round_div = [&](auto part) {
                return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
            };
            return complex<long long>(round_div(numer.real()), round_div(numer.imag()));
        };
        auto q = div(x, y), r = x - q * y;
        return {q, r};
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
tuple<T, T, bool> linear_diophantine_solution(T &a, T &b, T c) {
    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        auto div = [&](const T &x, const T &y) {
            auto numer = x * conj(y);
            auto denom = norm(y);
            auto round_div = [&](auto part) {
                return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
            };
            return complex<long long>(round_div(numer.real()), round_div(numer.imag()));
        };
        auto q = div(x, y), r = x - q * y;
        return {q, r};
    };

    auto [g, x, y] = extended_gcd(a, b);
    auto [q, r] = divmod(c, g);
    if (r != (T) 0) return {x, y, false};

    a /= g;
    b /= g;
    c /= g;
    x *= c;
    y *= c;
    return {x, y, true};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, x, y;
    cin >> a >> b >> x >> y;

    complex<long long> A(a, b), B(b, a), C(x, y);
    auto [X, Y, _] = linear_diophantine_solution(A, B, C);

    long double a1 = A.real(), b1 = A.imag(),
            a2 = B.real(), b2 = B.imag(),
            c1 = X.real(), c2 = X.imag(),
            c3 = Y.real(), c4 = Y.imag();

    vector<array<array<long double, 3>, 2>> eqns(4);
    eqns[0][0] = {a2, -b2, -c1};
    eqns[0][1] = {-a2, b2, c1};
    eqns[1][0] = {b2, a2, -c2};
    eqns[1][1] = {-b2, -a2, c2};
    eqns[2][0] = {-a1, b1, -c3};
    eqns[2][1] = {a1, -b1, c3};
    eqns[3][0] = {-b1, -a1, -c4};
    eqns[3][1] = {b1, a1, c4};

    unordered_set<Point<__int128>, Hash> candidates;
    auto add = [&](Point<long double> p) {
        Point<long long> q(p);
        for (int dx = -2; dx <= 2; dx++)
            for (int dy = -2; dy <= 2; dy++) candidates.emplace(q.x + dx, q.y + dy);
    };

    for (int mask = 0; mask < 1 << 4; mask++) {
        vector<Line<long double>> lines;
        for (int i = 0; i < 4; i++) {
            auto [ai, bi, ci] = eqns[i][(mask >> i) & 1];
            lines.emplace_back(Line(ai, bi, ci));
        }

        for (int i = 0; i < 4; i++)
            for (int j = i + 1; j < 4; j++)
                if (!intersects(lines[i], lines[j])) continue;
                else if (collinear(lines[i], lines[j]))
                    for (auto p : collinear_overlap(lines[i], lines[j])) add(p);
                else add(non_collinear_intersection(lines[i], lines[j]));
    }

    auto moves = [&](Point<long long> p) -> long long {
        return abs(p.x * a2 - p.y * b2 + c1) + abs(p.x * b2 + p.y * a2 + c2) + abs(-p.x * a1 + p.y * b1 + c3) + abs(-p.x * b1 - p.y * a1 + c4);
    };

    Point<long long> p{0, 0};
    auto m0 = moves(p);
    for (auto c : candidates) {
        auto m = moves(c);
        if (m0 > m) {
            m0 = m;
            p = c;
        }
    }

    bool change;
    do {
        change = false;
        for (int dx = -1; dx <= 1; dx++)
            for (int dy = -1; dy <= 1; dy++) {
                Point<long long> c{p.x + dx, p.y + dy};
                auto m = moves(c);
                if (m0 > m) {
                    m0 = m;
                    p = c;
                    change = true;
                }
            }
    } while (change);
    cout << m0;
}
