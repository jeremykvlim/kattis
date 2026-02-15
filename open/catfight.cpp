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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
pair<pair<int, int>, T> closest_pair(const vector<Point<T>> &points) {
    int n = points.size();

    vector<pair<Point<T>, int>> sorted(n);
    for (int i = 0; i < n; i++) sorted[i] = {points[i], i};
    sort(sorted.begin(), sorted.end(), [](auto p1, auto p2) { return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first; });

    T d = numeric_limits<T>::max();
    int a = -1, b = -1;
    auto update = [&](auto p1, auto p2) {
        auto dist = squared_dist(p1.first, p2.first);
        if (d > dist) {
            d = dist;
            a = p1.second;
            b = p2.second;
        }
    };

    auto sq = [](T v) -> T { return v * v; };
    auto cmp = [](auto p1, auto p2) { return p1.first.y < p2.first.y; };
    multiset<pair<Point<T>, int>, decltype(cmp)> ms(cmp);
    vector<typename decltype(ms)::const_iterator> its(n);
    for (int i = 0, j = 0; i < n; i++) {
        for (; j < i && sq(sorted[j].first.x - sorted[i].first.x) >= d; j++) ms.erase(its[j]);

        auto it = ms.upper_bound(sorted[i]);
        if (it != ms.begin()) {
            update(*prev(it), sorted[i]);
            for (auto p = prev(it); p != ms.begin() && sq(sorted[i].first.y - p->first.y) < d;) update(*(--p), sorted[i]);
        }
        for (; it != ms.end() && sq(it->first.y - sorted[i].first.y) < d; it++) update(*it, sorted[i]);
        its[i] = ms.emplace_hint(ms.upper_bound(sorted[i]), sorted[i]);
    }

    return {{a, b}, d};
}

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}

    bool contains(const Circle &c) const {
        return sgn(euclidean_dist(origin, c.origin) - radius + c.radius) < 0;
    }

    bool borders(const Circle &c) const {
        return !sgn(euclidean_dist(origin, c.origin) - radius + c.radius);
    }

    bool encloses(const Circle &c) const {
        return contains(c) || borders(c);
    }
};

template <typename T>
T area_of_circle_circle_intersections(const vector<Circle<T>> &circles) {
    int n = circles.size();

    T area = 0;
    for (int i = 0; i < n; i++) {
        vector<pair<T, T>> intervals{{0, 2 * M_PI}};
        for (int j = 0; j < n; j++)
            if (i != j) {
                auto v = circles[j].origin - circles[i].origin;
                T d = euclidean_dist(v);
                if (!sgn(d)) {
                    if (circles[j].encloses(circles[i])) continue;
                    intervals.clear();
                    break;
                }

                T bound = (circles[i].radius * circles[i].radius + d * d - circles[j].radius * circles[j].radius) / (2 * circles[i].radius * d);
                if (bound <= -1) continue;
                if (bound >= 1) {
                    intervals.clear();
                    break;
                }

                auto add = [&](const pair<T, T> &interval) {
                    vector<pair<T, T>> merged;
                    for (auto [l, r] : intervals) {
                        T x = max(l, interval.first), y = min(r, interval.second);
                        if (x < y) merged.emplace_back(x, y);
                    }
                    return merged;
                };

                T a = angle(v), theta = acos(bound),
                  l = fmod(fmod(a - theta, 2 * M_PI) + 2 * M_PI, 2 * M_PI), r = fmod(fmod(a + theta, 2 * M_PI) + 2 * M_PI, 2 * M_PI);

                if (l <= r) intervals = add({l, r});
                else {
                    auto temp = add({l, 2 * M_PI});
                    intervals = add({0, r});
                    intervals.insert(intervals.end(), temp.begin(), temp.end());
                }
                if (intervals.empty()) break;
            }

        for (auto [l, r] : intervals)
            if (l < r) {
                auto [p, R] = circles[i];
                area += 0.5 * (R * p.x * (sin(r) - sin(l)) + R * p.y * (cos(l) - cos(r)) + R * R * (r - l));
            }
    }
    return area;
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

    int n;
    double rf, rm;
    cin >> n >> rf >> rm;

    vector<Point<double>> male, female;
    while (n--) {
        char g;
        double x, y;
        cin >> g >> x >> y;

        if (g == 'M') male.emplace_back(x, y);
        else female.emplace_back(x, y);
    }

    double overlap = 0;
    if (male.size() >= 2) {
        auto [pair, d] = closest_pair(male);
        auto [a, b] = pair;
        Circle c1(male[a], rm), c2(male[b], rm);
        overlap = max(overlap, area_of_circle_circle_intersections(vector{c1, c2}));
    }
    if (female.size() >= 2) {
        auto [pair, d] = closest_pair(female);
        auto [a, b] = pair;
        Circle c1(female[a], rf), c2(female[b], rf);
        overlap = max(overlap, area_of_circle_circle_intersections(vector{c1, c2}));
    }
    if (!female.empty() && !male.empty()) {
        auto g1 = male, g2 = female;
        if (male.size() < female.size()) swap(g1, g2);

        KDTree<double> kdt(g1.size(), g1);
        auto d = 1e20;
        for (auto p : g2) d = min(d, kdt.nearest_neighbor_dist(p));
        Circle c1({0, 0}, rf), c2({sqrt(d), 0}, rm);
        overlap = max(overlap, area_of_circle_circle_intersections(vector{c1, c2}));
    }
    cout << fixed << setprecision(5) << overlap;
}
