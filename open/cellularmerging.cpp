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
        int l, r;
        Point<T> p;
        bool dir;
        T xl, xr, yl, yr;

        KDNode() {}
        KDNode(const Point<T> &p, bool dir, T xl, T xr, T yl, T yr) : p(p), dir(dir), xl(xl), xr(xr), yl(yl), yr(yr) {}
    };

    vector<Point<T>> points;
    vector<KDNode> KDT;

    KDTree(int n, const vector<Point<T>> &p) : points(p), KDT(n) {
        int i = 0;
        build(i, 0, n);
    }

    int build(int &i, int l, int r) {
        if (l >= r) return -1;

        int j = i++;
        T xl = points[l].x, xr = points[l].x, yl = points[l].y, yr = points[l].y;
        for (int k = l + 1; k < r; k++) {
            xl = min(xl, points[k].x);
            xr = max(xr, points[k].x);
            yl = min(yl, points[k].y);
            yr = max(yr, points[k].y);
        }

        bool dir = (xr - xl) < (yr - yl);
        int m = l + (r - l) / 2;
        nth_element(points.begin() + l, points.begin() + m, points.begin() + r, [dir](const auto &a, const auto &b) { return !dir ? a < b : !a < !b; });

        KDT[j] = {points[m], dir, xl, xr, yl, yr};
        KDT[j].l = build(i, l, m);
        KDT[j].r = build(i, m + 1, r);
        return j;
    }

    T nearest_neighbor_dist(const Point<T> &p) {
        return nearest_neighbor_dist(0, p);
    }

    T nearest_neighbor_dist(int i, const Point<T> &p, T dist = numeric_limits<T>::max()) {
        if (!~i || squared_dist(p, {min(KDT[i].xr, max(KDT[i].xl, p.x)), min(KDT[i].yr, max(KDT[i].yl, p.y))}) >= dist) return dist;

        dist = min(dist, squared_dist(p, KDT[i].p));
        T diff = !KDT[i].dir ? (p - KDT[i].p).x : (p - KDT[i].p).y;

        dist = nearest_neighbor_dist(diff <= 0 ? KDT[i].l : KDT[i].r, p, dist);
        if (diff * diff < dist) dist = nearest_neighbor_dist(diff <= 0 ? KDT[i].r : KDT[i].l, p, dist);
        return dist;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int z;
    while (cin >> z && z) {
        auto read = [&]() {
            string type;
            double x, y, s;
            cin >> type >> x >> y >> s;

            vector<Point<double>> polygon;
            if (type == "square") {
                for (int i = -floor(x / s) - 1; i <= ceil((z - x) / s) + 1; i++) {
                    auto x1 = x + i * s;
                    if (0 <= x1 && x1 <= z)
                        for (int j = -floor(y / s) - 1; j <= ceil((z - y)/ s) + 1; j++) {
                            auto y1 = y + j * s;
                            if (0 <= y1 && y1 <= z) polygon.emplace_back(x1, y1);
                        }
                }
            } else if (type == "diamond") {
                s /= sqrt(2);
                for (int i = -floor(x / s) - 1; i <= ceil((z - x) / s) + 1; i++) {
                    auto x1 = x + i * s;
                    if (0 <= x1 && x1 <= z)
                        for (int j = -floor(y / s) - 1; j <= ceil((z - y) / s) + 1; j++) {
                            if ((i + j) & 1) continue;
                            auto y1 = y + j * s;
                            if (0 <= y1 && y1 <= z) polygon.emplace_back(x1, y1);
                        }
                }
            } else if (type == "triangle") {
                auto s1 = s * sqrt(3);
                for (int i = -floor(x / s) - 1; i <= ceil((z - x) / s) + 1; i++) {
                    auto x1 = x + i * s;
                    if (0 <= x1 && x1 <= z)
                        for (int j = -floor(y / s1) - 1; j <= ceil((z - y)/ s1) + 1; j++) {
                            auto y1 = y + j * s1;
                            if (0 <= y1 && y1 <= z) polygon.emplace_back(x1, y1);
                        }
                }
                for (int i = -floor((x + s / 2) / s) - 1; i <= ceil((z - x - s / 2) / s) + 1; i++) {
                    auto x1 = x + s / 2 + i * s;
                    if (0 <= x1 && x1 <= z)
                        for (int j = -floor((y + s1 / 2) / s1) - 1; j <= ceil((z - y - s1 / 2) / s1) + 1; j++) {
                            auto y1 = y + s1 / 2 + j * s1;
                            if (0 <= y1 && y1 <= z) polygon.emplace_back(x1, y1);
                        }
                }
            } else if (type == "hex") {
                auto s1 = s * sqrt(3), s2 = s * sqrt(3) / 2;
                for (int i = -floor(2 * (s + x) / (3 * s)) - 1; i <= ceil(2 * (z + s - x) / (3 * s)) + 1; i++)
                    for (int j = -floor((s1 + y + i * s2) / s1) - 1; j <= ceil((z - y - i * s2) / s1) + 1; j++) {
                        auto x1 = x + s / 2 + i * 3 * s / 2, y1 = y + s2 + i * s2 + j * s1;
                        polygon.emplace_back(x1 - s / 2, y1 - s2);
                        polygon.emplace_back(x1 + s / 2, y1 - s2);
                        polygon.emplace_back(x1 + s / 2, y1 + s2);
                        polygon.emplace_back(x1 - s / 2, y1 + s2);
                        polygon.emplace_back(x1 + s, y1);
                        polygon.emplace_back(x1 - s, y1);
                    }
                erase_if(polygon, [&](auto p) { return !(0 <= p.x && p.x <= z && 0 <= p.y && p.y <= z); });
            }

            sort(polygon.begin(), polygon.end());
            polygon.erase(unique(polygon.begin(), polygon.end()), polygon.end());
            return polygon;
        };
        auto polygon1 = read(), polygon2 = read();
        if (polygon1.size() > polygon2.size()) swap(polygon1, polygon2);

        KDTree kdt(polygon1.size(), polygon1);
        auto dist = 1e30;
        for (auto &p : polygon2) {
            dist = min(dist, kdt.nearest_neighbor_dist(p));
            if (!sgn(dist)) break;
        }
        cout << fixed << setprecision(3) << sqrt(dist) << "\n";
    }
}