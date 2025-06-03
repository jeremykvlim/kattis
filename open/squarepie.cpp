#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
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

    bool operator<(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == -1 : sgn(y - p.y) == -1;
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == 1 : sgn(y - p.y) == 1;
        return x != p.x ? x > p.x : y > p.y;
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
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T doubled_signed_area_of_polygon(const vector<Point<T>> &polygon) {
    T area = 0;
    for (int i = 0; i < polygon.size(); i++) area += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
    return area;
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Line<int>> lines(n);
    unordered_map<int, vector<int>> x_to_y, y_to_x;
    int xl = INT_MAX, xr = INT_MIN, yl = INT_MAX, yr = INT_MIN;
    for (auto &[a, b] : lines) {
        cin >> a.x >> a.y >> b.x >> b.y;

        xl = min(xl, min(a.x, b.x));
        xr = max(xr, max(a.x, b.x));
        yl = min(yl, min(a.y, b.y));
        yr = max(yr, max(a.y, b.y));
        x_to_y[a.x].emplace_back(a.y);
        x_to_y[b.x].emplace_back(b.y);
        y_to_x[a.y].emplace_back(a.x);
        y_to_x[b.y].emplace_back(b.x);
    }

    for (auto &[y, xs] : y_to_x) {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
    }
    for (auto &[x, ys] : x_to_y) {
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
    }

    vector<Line<int>> edges;
    vector<Point<int>> points;
    for (auto [a, b] : lines)
        if (a.y == b.y) {
            int y = a.y;
            auto [xa, xb] = minmax(a.x, b.x);
            for (auto it = lower_bound(y_to_x[y].begin(), y_to_x[y].end(), xa); next(it) != upper_bound(y_to_x[y].begin(), y_to_x[y].end(), xb); it++) {
                edges.emplace_back(Point<int>{*it, y}, Point<int>{*next(it), y});
                points.emplace_back(edges.back().a);
                points.emplace_back(edges.back().b);
            }
        } else {
            int x = a.x;
            auto [ya, yb] = minmax(a.y, b.y);
            for (auto it = lower_bound(x_to_y[x].begin(), x_to_y[x].end(), ya); next(it) != upper_bound(x_to_y[x].begin(), x_to_y[x].end(), yb); it++) {
                edges.emplace_back(Point<int>{x, *it}, Point<int>{x, *next(it)});
                points.emplace_back(edges.back().a);
                points.emplace_back(edges.back().b);
            }
        }
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    int m = points.size();
    vector<vector<int>> adj_list(m);
    for (auto [a, b] : edges) {
        int u = lower_bound(points.begin(), points.end(), a) - points.begin(), v = lower_bound(points.begin(), points.end(), b) - points.begin();
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    for (int i = 0; i < m; i++)
        sort(adj_list[i].begin(), adj_list[i].end(), [&](int j, int k) {
                auto cmp = [&](int j) {
                    auto v = points[j] - points[i];
                    if (v.x > 0) return 0;
                    else if (v.y > 0) return 1;
                    else if (v.x < 0) return 2;
                    else return 3;
                };
                return cmp(j) < cmp(k);
             });

    vector<vector<bool>> visited(m);
    for (int v = 0; v < m; v++) visited[v].assign(adj_list[v].size(), false);

    vector<long long> areas;
    for (int v = 0; v < m; v++)
        for (int i = 0; i < adj_list[v].size(); i++)
            if (!visited[v][i]) {
                visited[v][i] = true;
                vector<Point<long long>> polygon{points[v]};
                for (int t = v, u = adj_list[v][i];;) {
                    polygon.emplace_back(points[t]);

                    int j = 0, s = adj_list[u].size();
                    while (j < s && adj_list[u][j] != t) j++;
                    j = (j - 1 + s) % s;
                    visited[u][j] = true;

                    if (u == v && adj_list[u][j] == adj_list[v][i]) break;
                    t = u;
                    u = adj_list[u][j];
                }
                if (polygon.size() < 3) continue;

                auto area = 0.5 * fabs(doubled_signed_area_of_polygon(polygon));
                if (area > 0) areas.emplace_back(area);
            }
    auto total_area = (long long) (xr - xl) * (yr - yl);
    sort(areas.rbegin(), areas.rend());
    areas.erase(areas.begin());
    for (auto a : areas) cout << fixed << setprecision(9) << (double) a / total_area << "\n";
}
