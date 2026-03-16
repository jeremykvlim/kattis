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
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
struct KDTree {
    struct KDNode {
        int l, r, pos;
        Point<T> p;
        bool dir;
        T xl, xr, yl, yr;

        KDNode() {}
        KDNode(int pos, const Point<T> &p, bool dir, T xl, T xr, T yl, T yr) : pos(pos), p(p), dir(dir), xl(xl), xr(xr), yl(yl), yr(yr) {}
    };

    vector<Point<T>> points;
    vector<KDNode> KDT;
    vector<int> order;

    KDTree(int n, const vector<Point<T>> &p) : points(p), KDT(n), order(n) {
        iota(order.begin(), order.end(), 0);
        int i = 0;
        build(i, 0, n);
    }

    int build(int &i, int l, int r) {
        if (l >= r) return -1;

        int j = i++;
        T xl = points[order[l]].x, xr = points[order[l]].x, yl = points[order[l]].y, yr = points[order[l]].y;
        for (int k = l + 1; k < r; k++) {
            xl = min(xl, points[order[k]].x);
            xr = max(xr, points[order[k]].x);
            yl = min(yl, points[order[k]].y);
            yr = max(yr, points[order[k]].y);
        }

        bool dir = (xr - xl) < (yr - yl);
        int m = l + (r - l) / 2;
        nth_element(order.begin() + l, order.begin() + m, order.begin() + r, [&](const auto &a, const auto &b) { return !dir ? points[a] < points[b] : !points[a] < !points[b]; });

        KDT[j] = {order[m], points[order[m]], dir, xl, xr, yl, yr};
        KDT[j].l = build(i, l, m);
        KDT[j].r = build(i, m + 1, r);
        return j;
    }

    array<pair<T, int>, 2> nearest_two_neighbors(const Point<T> &p, int pos) {
        return nearest_two_neighbors(0, p, pos);
    }

    array<pair<T, int>, 2> nearest_two_neighbors(int i, const Point<T> &p, int pos, array<pair<T, int>, 2> dist = {pair{numeric_limits<T>::max(), -1}, pair{numeric_limits<T>::max(), -1}}) {
        if (!~i || squared_dist(p, {min(KDT[i].xr, max(KDT[i].xl, p.x)), min(KDT[i].yr, max(KDT[i].yl, p.y))}) >= dist[1].first) return dist;

        if (KDT[i].pos != pos && KDT[i].pos != dist[0].second && KDT[i].pos != dist[1].second) {
            T d = squared_dist(p, KDT[i].p);
            if (dist[0].first > d) {
                dist[1] = dist[0];
                dist[0] = {d, KDT[i].pos};
            } else if (dist[1].first > d) dist[1] = {d, KDT[i].pos};
        }
        T diff = !KDT[i].dir ? (p - KDT[i].p).x : (p - KDT[i].p).y;

        dist = nearest_two_neighbors(diff <= 0 ? KDT[i].l : KDT[i].r, p, pos, dist);
        if (diff * diff < dist[1].first) dist = nearest_two_neighbors(diff <= 0 ? KDT[i].r : KDT[i].l, p, pos, dist);
        return dist;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    KDTree kdt(n, points);
    vector<pair<double, pair<int, int>>> candidates;
    for (int i = 0; i < n; i++) {
        auto [p1, p2] = kdt.nearest_two_neighbors(points[i], i);
        if (~p1.second) candidates.emplace_back(p1.first, minmax(i, p1.second));
        if (~p2.second) candidates.emplace_back(p2.first, minmax(i, p2.second));
    }
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end(), [&](auto &p1, auto &p2) { return p1.second == p2.second; }), candidates.end());
    cout << fixed << setprecision(6) << sqrt(candidates[0].first) << "\n" << sqrt(candidates[1].first);
}
