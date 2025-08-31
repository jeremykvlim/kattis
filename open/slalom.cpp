#include <bits/stdc++.h>
using namespace std;

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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Point<double>> points(2 * n - 1);
        vector<double> X(2 * n + 1);

        cin >> points[0].x >> points[0].y;
        X[0] = points[0].x;

        for (int i = 1; i < n; i++) {
            double y, x1, x2;
            cin >> y >> x1 >> x2;

            points[2 * i - 1] = {x1, y};
            points[2 * i] = {x2, y};
            X[2 * i - 1] = x1;
            X[2 * i] = x2;
        }

        double y, x1, x2;
        cin >> y >> x1 >> x2;

        X[2 * n - 1] = x1;
        X[2 * n] = x2;
        sort(X.begin(), X.end());
        X.erase(unique(X.begin(), X.end()), X.end());

        for (auto x : X)
            if (x1 <= x && x <= x2) points.emplace_back(x, y);

        vector<vector<pair<int, double>>> adj_list(points.size());
        for (int i = 0; i < 2 * n - 1; i++) {
            double l = INT_MIN, r = INT_MAX;

            auto pos = [](const Point<double> &a, const Point<double> &b) {
                auto v = b - a;
                return v.x / v.y;
            };

            for (int j = i + 1 + (i & 1); j < 2 * n - 1 && l <= r; j++) {
                auto curr = pos(points[i], points[j]);
                if (l <= curr && curr <= r) adj_list[i].emplace_back(j, euclidean_dist(points[i], points[j]));

                if (j & 1) r = min(r, curr);
                else l = max(l, curr);
            }

            for (int j = 2 * n - 1; j < points.size(); j++) {
                auto curr = pos(points[i], points[j]);
                if (l <= curr && curr <= r) adj_list[i].emplace_back(j, euclidean_dist(points[i], points[j]));
            }
        }

        vector<int> degree(points.size(), 0);
        for (int v = 0; v < points.size(); v++)
            for (auto [u, w] : adj_list[v]) degree[u]++;

        vector<double> dist(points.size(), INT_MAX);
        dist[0] = 0;
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, w] : adj_list[v]) {
                dist[u] = min(dist[u], dist[v] + w);
                if (!--degree[u]) q.emplace(u);
            }
        }

        cout << fixed << setprecision(7) << *min_element(dist.begin() + 2 * n - 1, dist.end()) << "\n";
    }
}
