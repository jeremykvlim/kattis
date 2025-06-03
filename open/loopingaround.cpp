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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<Point<int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;
        sort(points.begin(), points.end());

        unordered_map<int, set<Point<int>>> X, Y;
        for (int i = 0; i < n; i++) {
            auto [x, y] = points[i];
            X[y].emplace(x, i);
            Y[x].emplace(y, i);
        }

        vector<int> hori(n, -1), verti(n, -1);
        vector<bool> visited(n, false);

        auto dfs = [&](auto &&self, int v = 0) -> void {
            visited[v] = true;
            if (!visited[hori[v]]) self(self, hori[v]);
            if (!visited[verti[v]]) self(self, verti[v]);
        };

        multiset<int> active;
        for (int i = 0; i < n; i++) {
            auto [x, y] = points[i];

            if (!~hori[i]) {
                auto it1 = X[y].upper_bound({x, n});

                if (it1 == X[y].end()) goto next;
                else {
                    hori[i] = it1->y;
                    hori[it1->y] = i;
                    active.emplace(y);
                }
            } else active.erase(active.find(y));

            if (!~verti[i]) {
                auto it1 = Y[x].upper_bound({y, n});

                if (it1 == Y[x].end()) goto next;
                else {
                    auto it2 = active.upper_bound(y);

                    if (it2 != active.end() && *it2 < it1->x) goto next;
                    else {
                        verti[i] = it1->y;
                        verti[it1->y] = i;
                    }
                }
            }
        }
        dfs(dfs);

        if (all_of(visited.begin(), visited.end(), [&](bool b) { return b; })) {
            cout << "YES\n";
            continue;
        }
        next:;
        cout << "NO\n";
    }
}
