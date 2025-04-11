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
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<Point<int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    vector<vector<pair<int, int>>> adj_dir(n, vector<pair<int, int>>(4, {0, 0}));
    while (m--) {
        int i, j, k;
        cin >> i >> j >> k;
        i--;
        j--;

        for (int d = 0; d < 4; d++) {
            auto v = points[i] - points[j];
            if (dx[d] == sgn(v.x) && dy[d] == sgn(v.y)) {
                adj_dir[i][d ^ 2] = {j, k};
                adj_dir[j][d] = {i, k};
            }
        }
    }

    int s;
    char dir;
    cin >> s >> dir;
    s--;

    unordered_map<char, int> dtoi{{'E', 0}, {'N', 1}, {'W', 2}, {'S', 3}};
    for (int d = dtoi[dir], l = 1;;) {
        int k = 1e9;
        set<pair<int, int>> paths;
        while (!paths.count({s, d})) {
            paths.emplace(s, d);
            adj_dir[s][d].second -= l;
            k = min(k, adj_dir[s][d].second - 1);

            s = adj_dir[s][d].first;
            adj_dir[s][d ^ 2].second -= l;
            if (paths.count({s, d ^ 2})) k = min(k, adj_dir[s][d].second / 2 - 1);

            int count = adj_dir[s][d].second > 0;
            for (int i : {1, 3}) count += adj_dir[s][(d + i) % 4].second > 0;

            if (!count) {
                cout << points[s].x << " " << points[s].y;
                exit(0);
            }

            if (count != 3)
                for (int i : {1, 0, 3, 2})
                    if (adj_dir[s][(d + i) % 4].second) {
                        d = (d + i) % 4;
                        break;
                    }

            if (!k && l == 1) break;
        }
        l = max(1, k);
    }
}
