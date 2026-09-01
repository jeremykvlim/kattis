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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
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

    void update(const auto &p) {
        while (!this->empty() && this->back().y >= p.y) this->pop_back();
        while (this->size() > 1 && violates(p, this->back(), (*this)[this->size() - 2])) this->pop_back();
        this->emplace_back(p);
    }

    auto query() {
        return (*this)[this->size() - 2];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long xs, ys, xf, yf;
    int n;
    cin >> xs >> ys >> xf >> yf >> n;

    vector<array<long long, 3>> gates(n);
    for (auto &[l, r, y] : gates) cin >> l >> r >> y;
    sort(gates.begin(), gates.end(), [](auto a, auto b) { return a[2] > b[2]; });
    gates.push_back({xf, xf, yf});

    Point<long long> pos{xs, ys};
    MonotonicHull<long long, 1, true> left;
    MonotonicHull<long long, -1, true> right;
    left.emplace_back(pos);
    right.emplace_back(pos);

    double dist = 0;
    auto turn = [&](auto p) {
        dist += euclidean_dist(pos, p);
        pos = p;
        left.update(p);
        right.update(p);
    };

    auto process = [&] {
        while (left.size() > 1 && right.size() > 1 && cross(pos, left.query(), right.query()) < 0) {
            auto l = left.query(), r = right.query();
            turn(l.y > r.y ? l : r);
        }
    };

    for (auto [l, r, y] : gates) {
        left.add(Point{l, y});
        right.add(Point{r, y});
        process();
    }
    cout << fixed << setprecision(6) << dist + euclidean_dist(pos, {xf, yf});
}
