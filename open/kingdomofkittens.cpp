#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
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
bool collinear(const Point<T> &a, const Point<T> &b) {
    return !sgn(cross(a, b));
}

template <typename T>
bool between(const Point<T> &c, const Point<T> &a, const Point<T> &b) {
    return collinear(a - b, a - c) && dot(a - c, b - c) < 0;
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        T cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    for (auto p : points) {
        while (convex_hull.size() > 1 && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    convex_hull.pop_back();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<long long>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;

        auto convex_hull = monotone_chain(points, true);
        int size = convex_hull.size();

        if (size <= 2) {
            cout << "YES\n";
            continue;
        }

        auto next = [&](int i) {
            return (i + 1) % size;
        };

        auto forms_hull = [&]() {
            auto vertex_or_on_side = [&](Point<long long> c) {
                for (int i = 0; i < size; i++) {
                    auto a = convex_hull[i], b = convex_hull[next(i)];
                    if (a == c || b == c || between(c, a, b)) return true;
                }

                return false;
            };

            return all_of(points.begin(), points.end(), [&](auto c) { return vertex_or_on_side(c); });
        };

        if (size > 6 || !forms_hull()) {
            cout << "NO\n";
            continue;
        }

        vector<bool> on_side(size, false);
        for (int i = 0; i < size; i++) {
            auto a = convex_hull[i], b = convex_hull[next(i)];
            for (auto c : points)
                if (a != c && b != c && between(c, a, b)) on_side[i] = true;
        }

        auto valid = [&](int i, int j) -> bool {
            auto cross_product = cross(convex_hull[next(i)] - convex_hull[i], convex_hull[next(j)] - convex_hull[j]);
            if (cross_product == 0) return false;
            if (cross_product < 0) swap(i, j);

            for (int k = next(i); k != j; k = next(k))
                if (on_side[k] || next(k) != j) return false;

            int count = 0;
            for (int k = next(j); k != i; k = next(k)) count++;
            if (count <= 1) return true;
            if (count >= 4) return false;

            if (count == 2) {
                if (on_side[next(j)] && (on_side[next(j + 1)] || cross(convex_hull[next(j + 1)] - convex_hull[next(j)], convex_hull[next(i)] - convex_hull[i]) <= 0) ||
                    on_side[next(j + 1)] && cross(convex_hull[next(j)] - convex_hull[j], convex_hull[next(j + 2)] - convex_hull[next(j + 1)]) <= 0) return false;

                return true;
            }

            if (on_side[next(j)] || on_side[next(j + 2)]) return false;
            if (cross(convex_hull[next(j)] - convex_hull[j], convex_hull[next(j + 2)] - convex_hull[next(j + 1)]) > 0 &&
                cross(convex_hull[next(j + 2)] - convex_hull[next(j + 1)], convex_hull[next(i)] - convex_hull[i]) > 0)
                return true;
            return false;
        };

        for (int i = 0; i < size; i++)
            for (int j = i + 1; j < size; j++)
                if (valid(i, j)) {
                    cout << "YES\n";
                    goto next;
                }

        cout << "NO\n";
        next:;
    }
}
