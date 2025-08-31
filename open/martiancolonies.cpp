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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

template <typename T>
vector<vector<pair<int, T>>> kruskal(int n, vector<tuple<T, int, int>> edges) {
    DisjointSets dsu(n);
    sort(edges.begin(), edges.end());

    int count = 0;
    vector<vector<pair<int, T>>> adj_list(n);
    for (auto [w, u, v] : edges)
        if (dsu.unite(u, v)) {
            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
            if (++count == n - 1) break;
        }

    return adj_list;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, tc = 1;
    while (cin >> n && n) {
        vector<Point<double>> coords(n);
        vector<int> k(n);
        for (int i = 0; i < n; i++) cin >> coords[i].x >> coords[i].y >> k[i];

        vector<tuple<double, int, int>> edges;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) edges.emplace_back(euclidean_dist(coords[i], coords[j]) / 3, i, j);

        auto adj_list = kruskal(n, edges);
        vector<double> t(n, -1);
        t[0] = 0;
        auto dfs = [&](auto &&self, int v = 0) -> void {
            for (auto [u, w] : adj_list[v])
                if (t[u] == -1) {
                    t[u] = max(t[v], w);
                    self(self, u);
                }
        };
        dfs(dfs);

        double k_sum = 0, kt_sum = 0;
        for (int i = 0; i < n; i++) {
            k_sum += k[i];
            kt_sum += k[i] * t[i];
        }
        cout << "Colony Group: " << tc++ << " Average " << fixed << setprecision(2) << kt_sum / k_sum << "\n";
    }
}
