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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
T x_intercept(const Line<T> &l) {
    return l.a.x - l.a.y * (l.b.x - l.a.x) / (l.b.y - l.a.y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w;
    cin >> n >> w;

    vector<array<int, 3>> lines(n);
    for (auto &[x, yl, yh] : lines) cin >> x >> yl >> yh;

    int overlaps = 0;
    vector<double> lo, hi;
    for (int k = 0; k < n; k++) {
        vector<double> l, h;
        int count = 0;
        for (auto [x1, yl1, yh1] : lines) {
            auto [x2, yl2, yh2] = lines[k];
            if (x1 != x2) {
                if (yh2 < 0) {
                    yh2 = -yh2;
                    tie(yl1, yh1) = make_pair(-yh1, -yl1);
                }
                if (x1 < x2) swap(yl1, yh1);

                auto low = x_intercept(Line<double>({x2, yh2}, {x1, yl1})), high = x_intercept(Line<double>({x2, yh2}, {x1, yh1}));
                if (low > high) count++;

                if (low < -1e-9) l.emplace_back(low);
                else count--;

                if (high < -1e-9) h.emplace_back(high);
                else count++;
            }
        }
        sort(l.rbegin(), l.rend());
        sort(h.rbegin(), h.rend());

        if (count) overlaps++;
        for (int i = 0, j = 0; i < h.size() || j < l.size();)
            if (j == l.size() || i != h.size() && h[i] > l[j]) {
                if (!count++) hi.emplace_back(h[i]);
                i++;
            } else {
                if (!--count) lo.emplace_back(l[j]);
                j++;
            }
    }
    sort(lo.rbegin(), lo.rend());
    sort(hi.rbegin(), hi.rend());

    double sum = 0, prev = 0;
    for (int i = 0, j = 0; i < hi.size() || j < lo.size();) {
        bool single_cover = overlaps == n - 1;

        double x;
        if (j == lo.size() || i != hi.size() && hi[i] > lo[j]) {
            x = hi[i++];
            overlaps++;
        } else {
            x = lo[j++];
            overlaps--;
        }
        if (single_cover) sum += prev - x;
        prev = x;
    }

    if (overlaps < n - 1) cout << fixed << setprecision(6) << sum;
    else cout << -1;
}