#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-5;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

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
        return !approximately_equal(x, p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y);
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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
bool point_on_line(const Point<T> &p, const Line<T> &l, bool include_endpoints = true) {
    return !sgn(cross(l.b - l.a, p - l.a)) && (dot(l.a - p, l.b - p) < 0 || (include_endpoints && approximately_equal(dot(l.a - p, l.b - p), (T) 0)));
}

template <typename T>
bool parallel(const Line<T> &l1, const Line<T> &l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
bool collinear(const Line<T> &l1, const Line<T> &l2) {
    return parallel(l1, l2) && !sgn(cross(l1.b - l1.a, l2.a - l1.a));
}

template <typename T>
bool intersects(const Line<T> &l1, const Line<T> &l2) {
    return collinear(l1, l2) || !parallel(l1, l2);
}

template <typename T>
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * (cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> lamps(n), corners(4 * n);
    vector<int> e(n);
    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y >> e[i];

        lamps[i] = {x, y};
        corners[4 * i] = {x - 1, y - 1};
        corners[4 * i + 1] = {x - 1, y + 1};
        corners[4 * i + 2] = {x + 1, y - 1};
        corners[4 * i + 3] = {x + 1, y + 1};
    }

    vector<int> energy(1 << n, 0);
    for (int mask = 0; mask < 1 << n; mask++)
        for (int i = 0; i < n; i++)
            if ((mask >> i) & 1) energy[mask] += e[i];

    if (!energy.back()) {
        cout << 0;
        exit(0);
    }

    vector<vector<int>> masks(4 * n, vector<int>(4 * n)), adj_list(4 * n);
    vector<vector<double>> w(4 * n, vector<double>(4 * n, 0));
    for (int i = 0; i < 4 * n; i++)
        for (int j = i + 1; j < 4 * n; j++) {
            auto a = corners[i], b = corners[j];
            if (a == b) continue;
            auto c = a + b;
            Line<double> l1(a, b);
            for (int k = 0; k < n; k++) {
                if (c + Point(1, 0) == lamps[k] * 2 ||
                    c + Point(0, 1) == lamps[k] * 2 ||
                    c + Point(-1, 0) == lamps[k] * 2 ||
                    c + Point(0, -1) == lamps[k] * 2 ||
                    point_on_line(lamps[k], l1, false) ||
                    point_on_line(corners[4 * k], l1, false) ||
                    point_on_line(corners[4 * k + 1], l1, false) ||
                    point_on_line(corners[4 * k + 2], l1, false) ||
                    point_on_line(corners[4 * k + 3], l1, false)) goto next;

                Line<double> l2(corners[4 * k], corners[4 * k + 1]), l3(corners[4 * k], corners[4 * k + 2]),
                        l4(corners[4 * k + 3], corners[4 * k + 1]), l5(corners[4 * k + 3], corners[4 * k + 2]);
                for (auto l : {l2, l3, l4, l5}) {
                    if (!intersects(l1, l) || collinear(l1, l)) continue;
                    auto p = non_collinear_intersection(l1, l);
                    if (point_on_line(p, l1, false) && point_on_line(p, l)) goto next;
                }
            }
            adj_list[i].emplace_back(j);
            adj_list[j].emplace_back(i);
            w[i][j] = w[j][i] = euclidean_dist(a, b);
            if (a.y == b.y) continue;
            if (a.y > b.y) swap(a, b);
            for (int k = 0; k < n; k++)
                if (a.y < lamps[k].y && lamps[k].y <= b.y && cross(a, b, lamps[k]) > 0) {
                    masks[i][j] |= 1 << k;
                    masks[j][i] |= 1 << k;
                }
            next:;
        }

    double len = 1e20;
    priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<>> pq;
    for (int i = 0; i < 4 * n; i++) {
        for (int j = 0; j < n; j++)
            if (corners[i] + Point(1, 1) == lamps[j]) goto valid;
        continue;

        valid:;
        vector<vector<double>> dist(4 * n, vector<double>(1 << n, 1e20));
        dist[i][0] = 0;
        pq.emplace(0, make_pair(i, 0));
        while (!pq.empty()) {
            auto [d, p] = pq.top();
            pq.pop();

            auto [v, m1] = p;

            if (dist[v][m1] != d) continue;

            for (int u : adj_list[v]) {
                int m2 = m1 ^ masks[v][u];

                if (dist[u][m2] > d + w[v][u]) {
                    dist[u][m2] = d + w[v][u];
                    pq.emplace(d + w[v][u], make_pair(u, m2));
                }
            }
        }

        for (int mask = 1; mask < 1 << n; mask++)
            if (dist[i][mask] != 1e20 && energy[mask] * 2 == energy.back()) len = min(len, dist[i][mask]);
    }

    if (len == 1e20) cout << "IMPOSSIBLE";
    else cout << fixed << setprecision(6) << len;
}
