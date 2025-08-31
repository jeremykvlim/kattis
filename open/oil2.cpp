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
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Line<long long>> lines(n);
    for (auto &[a, b] : lines) {
        int x0, x1, y;
        cin >> x0 >> x1 >> y;

        if (x0 > x1) swap(x0, x1);
        a = {x0, y};
        b = {x1, y};
    }

    auto oil = 0LL;
    for (auto [a, b] : lines) {
        vector<Line<long long>> temp;
        for (auto [c, d] : lines)
            if (c.y != a.y) temp.emplace_back(c.y > a.y ? Line<long long>{{c.x - a.x, c.y - a.y}, {d.x - a.x, c.y - a.y}} :
                                                          Line<long long>{{a.x - d.x, a.y - c.y}, {a.x - c.x, a.y - c.y}});

        sort(temp.begin(), temp.end(), [&](auto &l1, auto &l2) { return cross(l1.a, l2.a) < 0; });

        priority_queue pq([](auto &l1, auto &l2) { return cross(l1.b, l2.b) > 0; }, vector<Line<long long>>());
        auto most = 0LL, sum = 0LL;
        for (auto l : temp) {
            auto [c, d] = l;
            pq.emplace(l);
            while (cross(pq.top().b, c) < 0) {
                auto [e, f] = pq.top();
                sum -= f.x - e.x;
                pq.pop();
            }
            sum += d.x - c.x;
            most = max(most, sum);
        }
        oil = max(oil, most + b.x - a.x);
    }
    cout << oil;
}
