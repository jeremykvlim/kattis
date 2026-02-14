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
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
vector<Point<T>> minkowski_sum(vector<Point<T>> &P, vector<Point<T>> &Q) {
    auto reorder = [&](auto &polygon) {
        int i = min_element(polygon.begin(), polygon.end()) - polygon.begin();
        rotate(polygon.begin(), polygon.begin() + i, polygon.end());
    };
    reorder(P);
    reorder(Q);

    int n = P.size(), m = Q.size();
    vector<Point<T>> R;
    if (n == 1 || m == 1) {
        for (auto p : P)
            for (auto q : Q) R.emplace_back(p + q);
        return R;
    }

    P.emplace_back(P[0]);
    P.emplace_back(P[1]);
    Q.emplace_back(Q[0]);
    Q.emplace_back(Q[1]);
    for (int i = 0, j = 0; i < n || j < m;) {
        R.emplace_back(P[i] + Q[j]);
        auto cp = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        if (cp >= 0) i++;
        if (cp <= 0) j++;
    }
    P.pop_back();
    P.pop_back();
    Q.pop_back();
    Q.pop_back();
    return R;
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        T cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    for (auto p : points) {
        while (convex_hull.size() > 1 && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    convex_hull.pop_back();
    return convex_hull;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    vector<Point<long long>> points(n);
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;

        if (!k) cin >> points[i].x >> points[i].y;
        else {
            adj_list[i].resize(k);
            for (int &j : adj_list[i]) {
                cin >> j;
                j--;
            }
        }
    }

    auto dfs = [&](auto &&self1, int v = 0) -> vector<Point<long long>> {
        if (adj_list[v].empty()) return {points[v]};

        int degree = adj_list[v].size();
        vector<vector<Point<long long>>> pos, neg;
        for (int u : adj_list[v]) {
            auto P = self1(self1, u), Q = P;
            for (auto &p : Q) p *= -1;
            pos.emplace_back(P);
            neg.emplace_back(Q);
        }

        auto dnc = [&](auto &&self2, int l, int r) -> pair<vector<Point<long long>>, vector<Point<long long>>> {
            if (l + 1 == r) return {neg[l], pos[l]};

            int m = l + (r - l) / 2;
            auto [neg_l, pos_l] = self2(self2, l, m);
            auto [neg_r, pos_r] = self2(self2, m, r);

            auto a = minkowski_sum(neg_l, neg_r), b = minkowski_sum(neg_l, pos_r), c = minkowski_sum(neg_r, pos_l);
            vector<Point<long long>> d;
            for (auto &p : b) d.emplace_back(p);
            for (auto &p : c) d.emplace_back(p);
            return {a, monotone_chain(d, true)};
        };
        return dnc(dnc, 0, degree).second;
    };
    auto quality = 0LL;
    for (auto p : dfs(dfs)) quality = max(quality, squared_dist(p));
    cout << quality;
}
