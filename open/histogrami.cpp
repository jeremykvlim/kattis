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
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, g;
    cin >> n >> m >> g;

    int max_x_h = -1, max_y_h = -1, max_x_s = -1, max_y_s = -1;
    vector<Point<int>> h(n), s(m);
    for (auto &[x, y] : h) {
        cin >> x >> y;

        max_x_h = max(max_x_h, x);
        max_y_h = max(max_y_h, y);
    }
    for (auto &[x, y] : s) {
        cin >> x >> y;

        max_x_s = max(max_x_s, x);
        max_y_s = max(max_y_s, y);
    }

    vector<int> y_h(max_x_h + 1, 0);
    for (int i = 1; i < n; i += 2)
        for (int x = h[i - 1].x + 1; x <= h[i].x; x++) y_h[x] = h[i].y;

    vector<vector<int>> y_s(max_x_s + 1);
    for (int i = 0; i < m; i++) y_s[s[i].x].emplace_back(s[i].y);

    int w = h[n - 1].x;
    FenwickTree<int> fw_count(max(max_y_h, max_y_s) + 1);
    FenwickTree<long long> fw_sum(max(max_y_h, max_y_s) + 1);
    vector<Point<int>> p(w + 1);
    vector<int> indices(max_y_s + 1, -1), lower_count(max_y_s + 1, 0), upper_count(max_y_s + 1, 0);
    vector<long long> d(w + 1, 1e18), lower_sum(max_y_s + 1, 0), upper_sum(max_y_s + 1, 0);
    d[0] = 0;
    for (int y : y_s[0]) indices[y] = 0;
    for (int x = 1; x <= w; x++) {
        fw_count.update(y_h[x], 1);
        fw_sum.update(y_h[x], y_h[x]);

        for (int y : y_s[x]) {
            int X = indices[y];

            int lc = fw_count.pref_sum(y - 1), uc = fw_count.pref_sum(max_y_h) - fw_count.pref_sum(y);
            auto ls = fw_sum.pref_sum(y - 1), us = fw_sum.pref_sum(max_y_h) - fw_sum.pref_sum(y);

            if (~X) {
                long long delta = g & 1 ? lc - lower_count[y] + uc - upper_count[y]
                                        : us - upper_sum[y] - ls + lower_sum[y] + (long long) y * (lc - lower_count[y]) - (long long) y * (uc - upper_count[y]);

                if (d[x] > d[X] + delta) {
                    d[x] = d[X] + delta;
                    p[x] = {X, y};
                }
            }
            tie(indices[y], lower_count[y], upper_count[y], lower_sum[y], upper_sum[y]) = tie(x, lc, uc, ls, us);
        }
    }

    cout << d[w] << "\n";
    vector<Point<int>> h_;
    for (Point<int> curr{w, 0}, prev; curr.x; curr = prev) {
        prev = p[curr.x];

        if (curr.y == prev.y) h_.back().x = prev.x;
        else {
            h_.emplace_back(curr.x, prev.y);
            h_.emplace_back(prev);
        }
    }
    reverse(h_.begin(), h_.end());
    cout << h_.size() << "\n";
    for (auto [x, y] : h_) cout << x << " " << y << "\n";
}
