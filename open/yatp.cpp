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

struct CentroidDecomposition {
    vector<vector<pair<int, int>>> adj_list;
    vector<bool> removed;
    vector<int> subtree_size;
    vector<long long> cost;

    CentroidDecomposition(int n) : adj_list(n), removed(n, false), subtree_size(n, 0), cost(n, LLONG_MAX) {}

    void add_edge(int u, int v, int w) {
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    void decompose(const vector<int> &p, int root = 1) {
        auto dfs1 = [&](auto &&self, int v, int prev = -1) -> void {
            subtree_size[v] = 1;
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u]) {
                    self(self, u, v);
                    subtree_size[v] += subtree_size[u];
                }
        };
        dfs1(dfs1, root);

        auto dfs2 = [&](auto &&self, int size, int v, int prev = -1) -> int {
            for (auto [u, w] : adj_list[v])
                if (u != prev && !removed[u])
                    if (2 * subtree_size[u] > size) return self(self, size, u, v);
            return v;
        };
        int c = dfs2(dfs2, subtree_size[root], root);
        removed[c] = true;

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
            if (p[v] != penalty) {
                hull.add(Point<long long>(penalty, dist));
                penalty = p[v];
                dist = d;
            } else dist = min(dist, d);
        }
        hull.add(Point<long long>(penalty, dist));
        for (auto [v, d] : component) cost[v] = min(cost[v], hull.query(p[v]) + d);

        for (auto [v, w] : adj_list[c])
            if (!removed[v]) decompose(p, v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n + 1);
    for (int i = 1; i <= n; i++) cin >> p[i];

    CentroidDecomposition cd(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int i, j, w;
        cin >> i >> j >> w;

        cd.add_edge(i, j, w);
    }
    cd.decompose(p);
    cout << accumulate(cd.cost.begin() + 1, cd.cost.end(), 0LL);
}
