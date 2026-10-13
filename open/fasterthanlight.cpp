
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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull : deque<Point<T>> {
    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (this->size() > 1 && violates((*this)[1], (*this)[0], p)) this->pop_front();
        this->emplace_front(p);
    }

    T query(const auto &p)  {
        auto f = [&](int i) {
            return cross(p, (*this)[i]);
        };

        int l = 0, r = this->size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if constexpr (sign > 0) {
                if (f(m) >= f(m - 1)) l = m;
                else r = m;
            } else {
                if (f(m) <= f(m - 1)) l = m;
                else r = m;
            }
        }
        return f(l);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 4>> rectangles(n);
    int xl = 0, xr = 1e9;
    for (auto &[x1, y1, x2, y2] : rectangles) {
        cin >> x1 >> y1 >> x2 >> y2;

        xl = max(xl, x1);
        xr = min(xr, x2);
    }

    if (xl <= xr) {
        cout << "possible";
        exit(0);
    }

    auto possible = [&](bool reflect) {
        vector<Point<long long>> lower_points, upper_points;
        for (auto [x1, y1, x2, y2] : rectangles)
            if (reflect) {
                lower_points.emplace_back(1e9 - x2, y2);
                upper_points.emplace_back(1e9 - x1, y1);
            } else {
                lower_points.emplace_back(x1, y2);
                upper_points.emplace_back(x2, y1);
            }
        sort(lower_points.begin(), lower_points.end());
        sort(upper_points.begin(), upper_points.end());

        MonotonicHull<long long, -1, true> lower;
        MonotonicHull<long long, 1, true> upper;
        for (int i = 0; i < n; i++) {
            if (!i || lower_points[i].x != lower_points[i - 1].x) lower.add(lower_points[i]);
            if (i + 1 == n || upper_points[i].x != upper_points[i + 1].x) upper.add(upper_points[i]);
        }

        auto check = [&](const Point<long long> &v = {1, 0}) {
            return upper.query(v) <= lower.query(v);
        };
        if (check()) return true;

        auto check_edges = [&](const auto &hull) {
            for (int i = 1; i < hull.size(); i++) {
                auto a = hull[i - 1], b = hull[i];
                if (a.y >= b.y && check(a - b)) return true;
            }
            return false;
        };
        return check_edges(upper) || check_edges(lower);
    };
    cout << (possible(false) || possible(true) ? "possible" : "impossible");
}
