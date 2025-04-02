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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T doubled_signed_area_of_polygon(const vector<Point<T>> &polygon) {
    T area = 0;
    for (int i = 0; i < polygon.size(); i++) area += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
    return area;
}

template <typename T>
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

template <typename T>
pair<bool, bool> point_in_polygon(const vector<Point<T>> &polygon, const Point<T> &p) {
    bool in = false;
    for (int i = 0; i < polygon.size(); i++) {
        auto a = polygon[i] - p, b = polygon[(i + 1) % polygon.size()] - p;
        if (a.y > b.y) swap(a, b);
        if (sgn(a.y) <= 0 && 0 < sgn(b.y) && sgn(cross(a, b)) < 0) in = !in;
        if (!sgn(cross(a, b)) && sgn(dot(a, b)) <= 0) return {false, true};
    }
    return {in, false};
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
T dist_from_point_to_line(const Point<T> &p, const Line<T> &l) {
    auto v = l.b - l.a, w = p - l.a;

    T d1 = dot(w, v);
    if (d1 <= 0) return euclidean_dist(p, l.a);

    T d2 = dot(v, v);
    if (d2 <= d1) return euclidean_dist(p, l.b);

    return euclidean_dist(p, l.a + v * d1 / d2);
}

template <typename T>
T dist_between_polygons(const vector<Point<T>> &P, const vector<Point<T>> &Q) {
    T dist = numeric_limits<T>::max();
    for (int i = 0; i < P.size(); i++) {
        auto a = P[i], b = P[(i + 1) % P.size()];
        for (int j = 0; j < Q.size(); j++) {
            auto c = Q[j], d = Q[(j + 1) % Q.size()];
            dist = min({dist, dist_from_point_to_line(a, Line(c, d)), dist_from_point_to_line(b, Line(c, d)),
                        dist_from_point_to_line(c, Line(a, b)), dist_from_point_to_line(d, Line(a, b))});
        }
    }
    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int f;
        double d;
        cin >> f >> d;

        Point<double> p1, p2;
        char _;
        cin >> _ >> p1.x >> p1.y >> _ >> _ >> p2.x >> p2.y >> _;

        vector<vector<Point<double>>> polygons(f);
        vector<double> areas(f), xl(f, 1e5), xr(f, -1e5), yl(f, 1e5), yr(f, -1e5);
        for (int i = 0; i < f; i++) {
            int v;
            cin >> v;

            polygons[i].resize(v);
            for (auto &[x, y] : polygons[i]) {
                cin >> _ >> x >> y >> _;

                xl[i] = min(xl[i], x);
                xr[i] = max(xr[i], x);
                yl[i] = min(yl[i], y);
                yr[i] = max(yr[i], y);
            }
            areas[i] = abs(doubled_signed_area_of_polygon(polygons[i])) / 2;
        }

        int s1 = -1, s2 = -1;
        for (int i = 0; i < f; i++) {
            if (point_in_polygon(polygons[i], p1).first) s1 = i;
            if (point_in_polygon(polygons[i], p2).first) s2 = i;
        }

        if (!~s1 || !~s2) {
            cout << "Scientists cannot meet\n";
            continue;
        }

        if (s1 == s2) {
            cout << fixed << setprecision(7) << areas[s1] << "\n";
            continue;
        }

        vector<vector<bool>> adj_matrix(f, vector<bool>(f, false));
        for (int i = 0; i < f; i++)
            for (int j = i + 1; j < f; j++) {
                double x = 0, y = 0;
                if (xl[j] > xr[i]) x = xl[j] - xr[i];
                else if (xl[i] > xr[j]) x = xl[i] - xr[j];
                if (yl[j] > yr[i]) y = yl[j] - yr[i];
                else if (yl[i] > yr[j]) y = yl[i] - yr[j];

                if (euclidean_dist(Point(x, y)) <= d && dist_between_polygons(polygons[i], polygons[j]) <= d) adj_matrix[i][j] = adj_matrix[j][i] = true;
            }

        vector<double> possible;
        for (int i = 0; i < f; i++)
            if (areas[i] <= min(areas[s1], areas[s2])) possible.emplace_back(areas[i]);
        
        if (possible.empty()) {
            cout << "Scientists cannot meet\n";
            continue;
        }
        
        sort(possible.begin(), possible.end());
        possible.erase(unique(possible.begin(), possible.end()), possible.end());

        int l = -1, r = possible.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            auto bfs = [&]() -> bool {
                if (areas[s1] < possible[m] || areas[s2] < possible[m]) return false;

                vector<bool> visited(f, false);
                visited[s1] = true;
                queue<int> q;
                q.emplace(s1);
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();

                    for (int u = 0; u < f; u++)
                        if (!visited[u] && adj_matrix[v][u])
                            if (areas[u] >= possible[m]) {
                                if (u == s2) return true;
                                visited[u] = true;
                                q.emplace(u);
                            }
                }
                return false;
            };
            
            if (bfs()) l = m;
            else r = m;
        }

        if (!~l) cout << "Scientists cannot meet\n";
        else cout << fixed << setprecision(7) << possible[l] << "\n";
    }
}
