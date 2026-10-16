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
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
auto euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    using U = common_type_t<T, double>;
    U x = (U) a.x - b.x, y = (U) a.y - b.y;
    return sqrt(x * x + y * y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int total = 0;
    vector<Point<int>> lamps(n), corners(4 * n);
    vector<int> e(n);
    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y >> e[i];

        total += e[i];
        lamps[i] = {x, y};
        corners[4 * i] = {x - 1, y - 1};
        corners[4 * i + 1] = {x - 1, y + 1};
        corners[4 * i + 2] = {x + 1, y - 1};
        corners[4 * i + 3] = {x + 1, y + 1};
    }
    sort(corners.begin(), corners.end());
    corners.erase(unique(corners.begin(), corners.end()), corners.end());

    if (!total) {
        cout << 0;
        exit(0);
    }

    int m = 1 << n;
    vector<int> energy(m);
    for (int mask = 1; mask < m; mask++) {
        int b = countr_zero((unsigned) mask);
        energy[mask] = energy[mask ^ (1 << b)] + e[b];
    }

    vector<int> targets;
    for (int mask = 1; mask < m - 1; mask++)
        if (2 * energy[mask] == total) targets.emplace_back(mask);

    if (targets.empty()) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    int V = corners.size();
    vector<vector<pair<int, int>>> adj_list(V);
    vector<vector<double>> adj_matrix(V, vector<double>(V));
    for (int i = 0; i < V; i++)
        for (int j = i + 1; j < V; j++) {
            auto a = corners[i], b = corners[j];

            auto w = euclidean_dist(a, b);
            adj_matrix[i][j] = adj_matrix[j][i] = w;

            bool valid = true;
            for (auto p : lamps) {
                if (max(a.x, b.x) <= p.x - 1 || min(a.x, b.x) >= p.x + 1 || max(a.y, b.y) <= p.y - 1 || min(a.y, b.y) >= p.y + 1) continue;
                auto v = b - a;
                if (abs(cross(a, b, p)) < abs(v.x) + abs(v.y)) {
                    valid = false;
                    break;
                }
            }
            if (!valid) continue;

            if (a.y > b.y) swap(a, b);
            int mask = 0;
            if (a.y != b.y)
                for (int k = 0; k < n; k++)
                    if (a.y < lamps[k].y && lamps[k].y <= b.y && cross(a, b, lamps[k]) > 0) mask |= 1 << k;

            adj_list[i].emplace_back(j, mask);
            adj_list[j].emplace_back(i, mask);
        }

    vector<int> roots;
    for (auto p : lamps) {
        int r = lower_bound(corners.begin(), corners.end(), Point<int>{p.x - 1, p.y - 1}) - corners.begin();
        if (find(roots.begin(), roots.end(), r) == roots.end()) roots.emplace_back(r);
    }

    auto len = 1e20;
    vector<vector<double>> dist(V, vector<double>(m));
    for (int r : roots) {
        for (auto &row : dist) fill(row.begin(), row.end(), 1e20);
        dist[r][0] = 0;
        priority_queue<tuple<double, double, int, int>, vector<tuple<double, double, int, int>>, greater<>> pq;
        pq.emplace(0, 0, r, 0);
        while (!pq.empty()) {
            auto [h, d, v, mv] = pq.top();
            pq.pop();

            if (h + 1e-9 >= len) break;
            if (d != dist[v][mv]) continue;

            if (v == r && mv && 2 * energy[mv] == total) {
                len = d;
                break;
            }

            for (auto [u, mask] : adj_list[v]) {
                auto w = adj_matrix[v][u];
                if (adj_matrix[u][r] + d + w + 1e-9 >= len) continue;
                int mu = mv ^ mask;
                if (dist[u][mu] > d + w) {
                    dist[u][mu] = d + w;
                    pq.emplace(adj_matrix[u][r] + d + w, d + w, u, mu);
                }
            }
        }
    }

    if (len == 1e20) cout << "IMPOSSIBLE";
    else cout << fixed << setprecision(6) << len;
}
