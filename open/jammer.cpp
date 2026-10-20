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

    Point<T> query(T x) {
        auto f = [&](const auto &p) {
            return p.x * x + p.y;
        };

        while (this->size() > 1 && f(this->back()) >= f((*this)[this->size() - 2])) this->pop_back();
        return this->back();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l;
    cin >> n >> l;

    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i].first;

        a[i].second = i;
    }
    sort(a.rbegin(), a.rend());

    vector<vector<int>> split(l + 1, vector<int>(n + 1));
    vector<long long> dp(n + 1, 0), temp(n + 1);
    for (int k = 1; k <= l; k++) {
        MonotonicHull<long long> hull;
        for (int i = k; i <= n; i++) {
            hull.add(Point<long long>{i - 1, -dp[i - 1]});
            auto [x, y] = hull.query(a[i - 1].first);
            temp[i] = (i - x) * a[i - 1].first - y;
            split[k][i] = x;
        }
        dp = temp;
    }

    vector<int> lanes(n);
    for (int i = n; l; l--) {
        int x = split[l][i];
        for (int j = x; j < i; j++) lanes[a[j].second] = l;
        i = x;
    }

    cout << fixed << setprecision(6) << (double) dp[n] / n << "\n";
    for (int lane : lanes) cout << lane << " ";
}
