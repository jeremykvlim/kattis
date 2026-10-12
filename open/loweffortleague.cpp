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

    T query(T x) {
        auto f = [&](const auto &p) {
            return p.x * x + p.y;
        };

        while (this->size() > 1 && f(this->back()) >= f((*this)[this->size() - 2])) this->pop_back();
        return f(this->back());
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r;
    cin >> r;

    int n = 1 << r;
    vector<int> s(n);
    for (int &si : s) cin >> si;

    vector<long long> dp(n), temp(n);
    auto dnc = [&](auto &&self, int l, int r) -> vector<int> {
        if (l + 1 == r) return {l};

        int m = l + (r - l) / 2;
        auto left = self(self, l, m), right = self(self, m, r);

        auto update = [&](const vector<int> &v1, const vector<int> &v2) {
            auto value = LLONG_MAX;
            for (int a = 0; int i : v2) {
                for (; a < v1.size() && s[v1[a]] <= s[i]; a++) value = min(value, dp[v1[a]]);
                temp[i] = a ? dp[i] + value : LLONG_MAX;
            }

            MonotonicHull<long long, -1, true> hull;
            for (int a = v1.size() - 1, b = v2.size() - 1; ~b; b--) {
                int i = v2[b];
                while (~a && s[v1[a]] > s[i]) {
                    int x = s[v1[a]];

                    value = LLONG_MAX;
                    do value = min(value, dp[v1[a--]]);
                    while (~a && s[v1[a]] == x);

                    hull.add(Point<long long>{-2 * x, value + (long long) x * x});
                }
                if (!hull.empty()) temp[i] = min(temp[i], dp[i] + (long long) s[i] * s[i] + hull.query(s[i]));
            }
        };
        update(right, left);
        update(left, right);

        vector<int> both;
        merge(left.begin(), left.end(), right.begin(), right.end(), back_inserter(both), [&](int i, int j) { return s[i] != s[j] ? s[i] < s[j] : i < j; });
        for (int i : both) dp[i] = temp[i];
        return both;
    };
    dnc(dnc, 0, n);
    cout << dp[0];
}

