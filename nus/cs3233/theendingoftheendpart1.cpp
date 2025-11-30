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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull {
    deque<Point<T>> dq;

    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (dq.size() > 1 && violates(dq[1], dq[0], p)) dq.pop_front();
        dq.emplace_front(p);
    }

    T query(auto q) {
        auto f = [&](const auto &p) {
            return p.x * q.x + p.y * q.y;
        };

        while (dq.size() > 1 && f(dq[dq.size() - 1]) >= f(dq[dq.size() - 2])) dq.pop_back();
        return dq[dq.size() - 1].x;
    }

    auto & operator[](int i) {
        return dq[i];
    }

    const auto & operator[](int i) const {
        return dq[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    long long prev;
    cin >> prev;

    vector<Point<__int128>> mono;
    while (r-- > 1) {
        long long l;
        cin >> l;

        mono.push_back({prev - l, 1});
        while (mono.size() > 1 && cross(mono[mono.size() - 2], mono.back()) < 0) {
            auto p = mono.back();
            mono.pop_back();

            mono.back() += p;
        }
        prev = l;
    }

    MonotonicHull<long long> hull;
    for (int i = 1; i <= c; i++) {
        long long d;
        cin >> d;

        hull.add(Point((long long) i, d));
    }

    string s;
    int col = 1;
    for (auto q : mono) {
        auto x = hull.query(q);
        if (x > col) s.append(x - col, 'E');
        col = x;
        s.append(q.y, 'N');
    }
    if (col < c) s.append(c - col, 'E');
    cout << s;
}
