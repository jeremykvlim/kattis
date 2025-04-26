#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    auto xl = 1e7, xr = -1e7, yl = 1e7, yr = -1e7;
    vector<Point<double>> points(n);
    for (auto &[x, y] : points) {
        cin >> x >> y;

        xl = min(xl, x);
        xr = max(xr, x);
        yl = min(yl, y);
        yr = max(yr, y);
    }

    vector<double> R_l(n, -1);
    auto valid = [&](double R) -> bool {
        unordered_map<Point<int>, vector<int>, Hash> grid;
        for (int i = 0; i < n; i++) {
            Point<int> cell = (points[i] - Point<double>{xl, yl}) / (2 * R);
            grid[cell].emplace_back(i);
        }

        for (int i = 0; i < n; i++) {
            if (R_l[i] > R) continue;

            Point<int> p = (points[i] - Point<double>{xl, yl}) / (2 * R);
            vector<Point<double>> vectors;
            int count = 1;
            for (int dx = -1; dx <= 1; dx++)
                for (int dy = -1; dy <= 1; dy++) {
                    auto cell = p + Point{dx, dy};
                    if (grid.count(cell))
                        for (int j : grid[cell]) {
                            if (i == j) continue;

                            auto v = points[j] - points[i];
                            if (euclidean_dist(v) <= 2 * R + 1e-8) {
                                vectors.emplace_back(v);
                                count++;
                            }
                        }
                }

            if (count < k) {
                R_l[i] = max(R_l[i], R);
                continue;
            }

            vector<pair<double, bool>> intervals;
            int overlaps = 1;
            for (auto v : vectors) {
                auto a = angle(v), theta = acos(euclidean_dist(v) / (2 * R));
                intervals.emplace_back(a - theta, false);
                intervals.emplace_back(a + theta, true);
            }
            sort(intervals.begin(), intervals.end(), [&](const auto &p1, const auto &p2) { return !approximately_equal(p1.first, p2.first, 1e-8) ? p1.first < p2.first : p1.second < p2.second; });

            for (auto [angle, end] : intervals) {
                if (!end) overlaps++;
                else overlaps--;

                if (overlaps >= k) return true;
            }
            R_l[i] = max(R_l[i], R);
        }
        return false;
    };

    double l = 0, r = sqrt((xr - xl) * (xr - xl) + (yr - yl) * (yr - yl)) / 2 + 1, m;
    while (l + 1e-8 < r && l + l * 1e-8 < r) {
        m = l + (r - l) / 2;

        if (valid(m)) r = m;
        else l = m;
    }
    cout << fixed << setprecision(2) << m;
}
