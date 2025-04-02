#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
double squared_dist(const Point<T> &a, const Point<T> &b) {
    return (double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y);
}

template <typename T>
pair<pair<int, int>, double> closest_pair(const vector<Point<T>> &points) {
    int n = points.size();

    vector<pair<Point<T>, int>> sorted(n);
    for (int i = 0; i < n; i++) sorted[i] = {points[i], i};
    sort(sorted.begin(), sorted.end(), [](auto p1, auto p2) {return p1.first == p2.first ? p1.second < p2.second : p1.first < p2.first;});

    auto d = DBL_MAX;
    int a = -1, b = -1;
    auto update = [&](auto p1, auto p2) {
        auto dist = squared_dist(p1.first, p2.first);
        if (d > dist) {
            d = dist;
            a = p1.second;
            b = p2.second;
        }
    };

    auto sq = [](double v) -> double {return v * v;};
    auto cmp = [](auto p1, auto p2) {return p1.first.y < p2.first.y;};
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
};

template <typename T>
T circle_intersection_area(const Circle<T> &c1, const Circle<T> &c2) {
    T d = euclidean_dist(c1.origin, c2.origin);

    if (d >= c1.radius + c2.radius) return 0;

    auto [rmin, rmax] = minmax(c1.radius, c2.radius);
    if (d <= rmax - rmin) return M_PI * rmin * rmin;

    auto r1sq = c1.radius * c1.radius, r2sq = c2.radius * c2.radius,
         alpha = acos((d * d + r1sq - r2sq) / (2 * d * c1.radius)) * 2,
         beta = acos((d * d + r2sq - r1sq) / (2 * d * c2.radius)) * 2;
    return 0.5 * (r1sq * (alpha - sin(alpha)) + r2sq * (beta - sin(beta)));
}

template <typename T>
struct KDTree {
    struct KDNode {
        Point<T> p;
        bool dir;

        KDNode() {}
        KDNode(const Point<T> &p, bool dir = false) : p(p), dir(dir) {}
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
        KDT[j] = {points[m], dir};
        auto &[cl, cr] = children[j];
        cl = build(i, l, m, !dir);
        cr = build(i, m + 1, r, !dir);
        return j;
    }

    double nearest_neighbour_dist(const Point<T> &p) {
        return nearest_neighbour_dist(0, p);
    }

    double nearest_neighbour_dist(int i, const Point<T> &p) {
        if (i == -1) return 1e30;

        auto dist = squared_dist(p, KDT[i].p);
        auto diff = !KDT[i].dir ? (p - KDT[i].p).x : (p - KDT[i].p).y;

        auto [cl, cr] = children[i];
        dist = min(dist, nearest_neighbour_dist(diff <= 0 ? cl : cr, p));
        if (diff * diff < dist) dist = min(dist, nearest_neighbour_dist(diff <= 0 ? cr : cl, p));
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
        overlap = max(overlap, circle_intersection_area(c1, c2));
    }
    if (female.size() >= 2) {
        auto [pair, d] = closest_pair(female);
        auto [a, b] = pair;
        Circle c1(female[a], rf), c2(female[b], rf);
        overlap = max(overlap, circle_intersection_area(c1, c2));
    }
    if (!female.empty() && !male.empty()) {
        auto g1 = male, g2 = female;
        if (male.size() >= female.size()) swap(g1, g2);

        KDTree<double> kdt(g1.size(), g1);
        auto d = 1e30;
        for (auto p : g2) d = min(d, kdt.nearest_neighbour_dist(p));
        Circle c1({0, 0}, rf), c2({sqrt(d), 0}, rm);
        overlap = max(overlap, circle_intersection_area(c1, c2));
    }
    cout << fixed << setprecision(5) << overlap;
}
