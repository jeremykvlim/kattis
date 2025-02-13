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
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
struct KDTree {
    struct KDNode {
        Point<T> p;
        T xl, xr, yl, yr;
        int size;

        KDNode() {}
        KDNode(const Point<T> &p) : p(p), xl(p.x), xr(p.x), yl(p.y), yr(p.y), size(1) {}

        auto operator+=(const KDNode &node) {
            xl = min(xl, node.xl);
            xr = max(xr, node.xr);
            yl = min(yl, node.yl);
            yr = max(yr, node.yr);
            size += node.size;
        }
    };

    vector<Point<T>> points;
    vector<KDNode> KDT;
    vector<pair<int, int>> children;

    KDTree(int n, const vector<Point<T>> &p) : points(p), KDT(n), children(n, {-1, -1}) {
        int i = 0;
        build(i, 0, n);
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

    int points_in_half_plane(const Line<T> &l) {
        return points_in_half_plane(0, l.a - l.b, cross(l.a, l.b));
    }

    int points_in_half_plane(int i, const Point<T> &v, const T &c0) {
        if (i == -1) return 0;

        Point<T> pll(KDT[i].xl, KDT[i].yl), plr(KDT[i].xl, KDT[i].yr), prl(KDT[i].xr, KDT[i].yl), prr(KDT[i].xr, KDT[i].yr);
        T c1 = cross(pll, v), c2 = cross(plr, v), c3 = cross(prl, v), c4 = cross(prr, v);
        if (c0 + min({c1, c2, c3, c4}) > 0) return 0;
        if (c0 + max({c1, c2, c3, c4}) <= 0) return KDT[i].size;

        auto [cl, cr] = children[i];
        return points_in_half_plane(cl, v, c0) + points_in_half_plane(cr, v, c0) + (c0 + cross(KDT[i].p, v) <= 0);
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
            if (i != j) outside[i][j] = kdt.points_in_half_plane({polygon[i], polygon[j]});

    vector<vector<vector<int>>> memo(m, vector<vector<int>>(m, vector<int>(k + 1, -1)));
    auto dp = [&](auto &&self, int i, int l, int k) {
        if (!~k || i - l > m) return n;
        if (i - l == m) return 0;

        if (~memo[i % m][l][k]) return memo[i % m][l][k];
        int stolen = n;
        for (int j = i + 1; j < i + m; j++) stolen = min(stolen, outside[i % m][j % m] + self(self, j, l, k - 1));
        return memo[i % m][l][k] = stolen;
    };

    int stolen = n;
    for (int i = 0; i < m / k; i++) stolen = min(stolen, dp(dp, i, i, k));
    cout << n - stolen;
}
