#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}
};

vector<bool> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n, 0), low(n, 0);
    vector<bool> cutpoint(n, false);
    int count = 0;

    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        int children = 0;
        for (int u : adj_list[v])
            if (u != prev) {
                if (!order[u]) {
                    children++;
                    self(self, u, v);
                    low[v] = min(low[v], low[u]);
                    if (prev != -1 && low[u] >= order[v]) cutpoint[v] = true;
                } else low[v] = min(low[v], order[u]);
            }
        if (prev == -1 && children > 1) cutpoint[v] = true;
    };
    dfs(dfs, n - 1);

    return cutpoint;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int g, h;
    cin >> g >> h;

    vector<Point<long long>> glades(g);
    for (auto &[x, y] : glades) cin >> x >> y;

    vector<Circle<long long>> hills(h);
    for (auto &c : hills) cin >> c.origin.x >> c.origin.y >> c.radius;

    vector<vector<int>> adj_list(g);
    for (int v = 0; v < g; v++) {
        vector<tuple<double, int, long long, int>> sweep;
        for (int u = 0; u < g; u++)
            if (v != u) sweep.emplace_back(fmod(angle(glades[u] - glades[v]) + 2 * M_PI, 2 * M_PI), 1, squared_dist(glades[v], glades[u]), u);

        vector<bool> active(h, false);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        for (int i = 0; i < h; i++) {
            auto [o, radius] = hills[i];
            auto d2 = squared_dist(glades[v], o);
            if (d2 <= radius * radius) continue;

            auto d = d2 - radius * radius;
            auto a = fmod(angle(o - glades[v]) + 2 * M_PI, 2 * M_PI), angular_radius = atan2(radius, sqrt(d)),
                 al = fmod(a - angular_radius + 2 * M_PI, 2 * M_PI), ar = fmod(a + angular_radius + 2 * M_PI, 2 * M_PI);

            if (al > ar + 1e-8) {
                pq.emplace(d, i);
                active[i] = true;
            }

            sweep.emplace_back(al, 0, d, i);
            sweep.emplace_back(ar, 2, d, i);
        }
        sort(sweep.begin(), sweep.end());

        auto al = -1.;
        for (auto [ar, type, d, i] : sweep)
            if (!type) {
                pq.emplace(d, i);
                active[i] = true;
            } else if (type == 1 && al + 1e-8 < ar) {
                while (!pq.empty() && !active[pq.top().second]) pq.pop();
                if (pq.empty() || pq.top().first > d) adj_list[v].emplace_back(i);
                al = ar;
            } else active[i] = false;
    }

    auto cutpoint = tarjan(g, adj_list);
    dynamic_bitset<> visited(g);
    visited[g - 1] = true;
    auto dfs = [&](auto &&self, int v, int prev) -> void {
        visited[v] = true;
        if (!cutpoint[v]) {
            for (int u : adj_list[v])
                if (u != prev && !visited[u]) self(self, u, v);
        }
    };
    for (int v : adj_list[g - 1])
        if (!visited[v]) dfs(dfs, v, g - 1);

    for (int v = visited.find_first(); v < g - 1; v = visited.find_next(v)) cout << v + 1 << " ";
}
