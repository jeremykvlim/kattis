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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
bool parallel(const Line<T> &l1, const Line<T> &l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Line<long double>> lines(n);
    for (auto &[l1, l2] : lines) cin >> l1.x >> l1.y >> l2.x >> l2.y;

    int m = n * n * n;
    vector<vector<int>> indices(n, vector<int>(n, -1));
    vector<bool> valid(m, false);
    auto index = [&](int a, int b, int c) {
        return (a * n + b) * n + c;
    };
    vector<array<int, 3>> segments;
    for (int a = 0; a < n; a++) {
        map<Point<long double>, vector<int>> order;
        for (int b = 0; b < n; b++)
            if (!parallel(lines[a], lines[b])) order[non_collinear_intersection(lines[a], lines[b])].emplace_back(b);

        for (int id = 0; auto &[p, v] : order) {
            for (int b : v) indices[a][b] = id;
            id++;
        }

        for (int b = 0; b < n; b++)
            if (~indices[a][b])
                for (int c = 0; c < n; c++)
                    if (~indices[a][c]) {
                        valid[index(a, b, c)] = indices[a][b] != indices[a][c];
                        segments.push_back({a, b, c});
                    }
    }

    vector<vector<bool>> intersect(m, vector<bool>(m, false));
    for (int s1 = 1; s1 < segments.size(); s1++) {
        auto &[a, b, c] = segments[s1];
        auto [l1, r1] = minmax(indices[a][b], indices[a][c]);
        int i = index(a, b, c);
        for (int s2 = 0; s2 < s1; s2++) {
            auto &[d, e, f] = segments[s2];
            auto [l2, r2] = minmax(indices[d][e], indices[d][f]);
            int j = index(d, e, f);
            intersect[i][j] = intersect[j][i] = l1 <= indices[a][d] && indices[a][d] <= r1 &&
                                                l2 <= indices[d][a] && indices[d][a] <= r2;
        }
    }

    vector<int> order{n - 1};
    auto dfs = [&](auto &&self, unsigned mask) -> int {
        int s = order.size();
        auto check = [&](int a, int b, int c) {
            int i = index(a, b, c);
            if (!valid[i]) return false;

            for (int k = 2; k < s; k++)
                if (order[k - 1] != a && order[k - 1] != b && order[k - 1] != c)
                    if (intersect[i][index(order[k - 1], order[k], order[k - 2])]) return false;
            return true;
        };
        if (!mask) return check(order[s - 1], order[s - 2], order[0]) && check(order[0], order[1], order[s - 1]);

        int count = 0;
        for (auto m = mask; m; m &= m - 1) {
            int i = countr_zero(m);
            if (s < 2 || check(order[s - 1], order[s - 2], i)) {
                order.emplace_back(i);
                count += self(self, mask ^ (1 << i));
                order.pop_back();
            }
        }
        return count;
    };
    cout << dfs(dfs, (1 << (n - 1)) - 1) / 2;
}
