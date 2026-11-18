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

template <typename T, int sign = -1, bool collinear = false>
struct MonotonicHull : deque<Point<T>> {
    bool violates(const auto &a, const auto &b, const auto &c) {
        auto cp = cross(a, b, c);
        if constexpr (sign < 0) cp = -cp;
        return collinear ? cp >= 0 : cp > 0;
    }

    void add(const auto &p) {
        while (this->size() > 1 && violates((*this)[1], (*this)[0], p)) this->pop_front();
        this->emplace_front(p);
    }

    T query(T x) {
        auto f = [&](const auto &p) {
            return p.x * x + p.y;
        };

        T v = numeric_limits<T>::max();
        for (auto p : *this) v = min(v, f(p));
        return v;
    }
};

template <typename T, typename F>
void centroid_decomposition(const vector<vector<pair<int, T>>> &adj_list, F &&f) {
    vector<int> subtree_size(adj_list.size());
    vector<bool> removed(adj_list.size(), false);
    auto decompose = [&](auto &&self, int root = 1) -> void {
        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            subtree_size[v] = 1;
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) {
                    self(self, u, v);
                    subtree_size[v] += subtree_size[u];
                }
        };
        dfs(dfs, root);

        int c = root, size = subtree_size[root];
        for (int prev = -1, next = -1;; prev = c, c = next, next = -1) {
            for (auto [u, w] : adj_list[c])
                if (u != prev && !removed[u] && 2 * subtree_size[u] > size) {
                    next = u;
                    break;
                }
            if (!~next) break;
        }
        removed[c] = true;

        f(c, removed);
        for (auto [v, w] : adj_list[c])
            if (!removed[v]) self(self, v);
    };
    decompose(decompose);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n + 1);
    for (int i = 1; i <= n; i++) cin >> p[i];

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<long long> cost(n + 1, LLONG_MAX);
    centroid_decomposition(adj_list, [&](int c, const auto &removed) {
        vector<pair<int, long long>> component;
        auto dfs3 = [&](auto &&self, int v, int prev = -1, long long d = 0) -> void {
            component.emplace_back(v, d);
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) self(self, u, v, d + w);
        };
        dfs3(dfs3, c);

        sort(component.begin(), component.end(), [&](auto p1, auto p2) { return p[p1.first] < p[p2.first]; });
        MonotonicHull<long long> hull;
        int penalty = p[component[0].first];
        auto dist = component[0].second;
        for (int i = 1; i < component.size(); i++) {
            auto [v, d] = component[i];
            if (penalty != p[v]) {
                hull.add(Point<long long>(penalty, dist));
                penalty = p[v];
                dist = d;
            } else dist = min(dist, d);
        }
        hull.add(Point<long long>(penalty, dist));
        for (auto [v, d] : component) cost[v] = min(cost[v], hull.query(p[v]) + d);
    });
    cout << accumulate(cost.begin() + 1, cost.end(), 0LL);
}
