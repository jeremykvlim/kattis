#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

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

    struct Hash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
struct KDTree {
    struct KDNode {
        Point<T> p;
        T xl, xr, yl, yr;
        int subtree_size;

        KDNode() {}
        KDNode(const Point<T> &p) : p(p), xl(p.x), xr(p.x), yl(p.y), yr(p.y), subtree_size(1) {}

        auto operator+=(const KDNode &node) {
            xl = min(xl, node.xl);
            xr = max(xr, node.xr);
            yl = min(yl, node.yl);
            yr = max(yr, node.yr);
            subtree_size += node.subtree_size;
        }
    };

    vector<Point<T>> points;
    vector<KDNode> KDT;
    vector<pair<int, int>> children;

    KDTree(int n, const vector<Point<T>> &p) : points(p), KDT(n), children(n, {-1, -1}) {
        int i = 0;
        build(i, 0, points.size());
    }

    int build(int &i, int l, int r, bool dir = false) {
        if (l >= r) return -1;

        int m = l + (r - l) / 2;
        nth_element(points.begin() + l, points.begin() + m, points.begin() + r, [dir](const auto &a, const auto &b) {return !dir ? a.x < b.x : a.y < b.y;});

        int j = i++;
        KDT[j] = points[m];
        auto &[cl, cr] = children[j];
        cl = build(i, l, m, !dir);
        cr = build(i, m + 1, r, !dir);
        if (~cl) KDT[j] += KDT[cl];
        if (~cr) KDT[j] += KDT[cr];
        return j;
    }

    int query(const Point<T> &p1, const Point<T> &p2) {
        return query(0, p1, p2);
    }

    int query(int i, const Point<T> &p1, const Point<T> &p2) {
        if (i == -1) return 0;

        Point<T> v = p1 - p2, pll(KDT[i].xl, KDT[i].yl), plr(KDT[i].xl, KDT[i].yr), prl(KDT[i].xr, KDT[i].yl), prr(KDT[i].xr, KDT[i].yr);
        T cross_product = cross(p1, p2), b1 = cross(pll, v), b2 = cross(plr, v), b3 = cross(prl, v), b4 = cross(prr, v);
        if (cross_product + min({b1, b2, b3, b4}) > 0) return 0;
        if (cross_product + max({b1, b2, b3, b4}) <= 0) return KDT[i].subtree_size;

        int count = cross_product + cross(KDT[i].p, v) <= 0;
        auto [cl, cr] = children[i];
        return count + query(cl, p1, p2) + query(cr, p1, p2);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    k = min(m, k);

    vector<Point<long long>> onions(n), polygon(m);
    for (auto &[x, y] : onions) cin >> x >> y;
    for (auto &[x, y] : polygon) cin >> x >> y;
    reverse(polygon.begin(), polygon.end());

    KDTree<long long> kdt(n, onions);
    vector<vector<int>> outside(m, vector<int>(m, 0));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            if (i != j) outside[i][j] = kdt.query(polygon[i], polygon[j]);

    vector<vector<vector<int>>> memo(m, vector<vector<int>>(m, vector<int>(k + 1, -1)));
    auto dp = [&](auto &&self, int l, int r, int upgrades) {
        if (!~upgrades || r - l > m) return n;
        if (r - l == m) return 0;

        if (~memo[l % m][r % m][upgrades]) return memo[l % m][r % m][upgrades];
        int stolen = n;
        for (int i = r + 1; i < r + m; i++) stolen = min(stolen, outside[r % m][i % m] + self(self, l, i, upgrades - 1));
        return memo[l % m][r % m][upgrades] = stolen;
    };

    int stolen = n;
    for (int i = 0; i < m / k; i++) stolen = min(stolen, dp(dp, i, i, k));
    cout << n - stolen;
}
