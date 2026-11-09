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
T manhattan_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

struct RollbackDisjointSets {
    vector<int> sets;
    vector<pair<int, int>> history;

    int find(int v) {
        while (sets[v] >= 0) v = sets[v];
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        history.emplace_back(v_set, sets[v_set]);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    int record() {
        return history.size();
    }

    void rollback(int version) {
        while (record() > version) {
            auto [v_set, s] = history.back();
            history.pop_back();

            int u_set = sets[v_set];
            sets[u_set] -= s;
            sets[v_set] = s;
        }
    }

    void delete_history(int version = 0) {
        history.resize(version);
    }

    RollbackDisjointSets(int n) : sets(n, -1) {}
};

template <typename T>
pair<T, vector<int>> edmonds_dense(int n, vector<vector<T>> &adj_matrix_transpose, int root = 0) {
    vector<vector<pair<int, int>>> edge(n, vector<pair<int, int>>(n, {-1, -1}));
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++) edge[u][v] = {v, u};

    RollbackDisjointSets rdsu(n);
    vector<pair<int, int>> chosen_edge(n, {-1, -1});
    stack<tuple<int, int, vector<pair<int, int>>>> history;
    vector<bool> cycle(n, false);
    vector<int> cycle_nodes, rep(n), active(n), seen(n, -1);
    iota(rep.begin(), rep.end(), 0);
    iota(active.begin(), active.end(), 0);
    seen[root] = root;
    T len = 0;
    for (int s = 0; s < n; s++) {
        int v = rep[rdsu.find(s)];
        while (!~seen[v]) {
            T w = numeric_limits<T>::max();
            for (int u : active)
                if (w > adj_matrix_transpose[v][u]) {
                    w = adj_matrix_transpose[v][u];
                    chosen_edge[v] = edge[v][u];
                }
            if (w == numeric_limits<T>::max()) return {0, {}};

            seen[v] = s;
            len += w;
            for (int u : active)
                if (u != v) adj_matrix_transpose[v][u] -= w;

            int r = rep[rdsu.find(chosen_edge[v].first)];
            if (seen[r] != s) {
                v = r;
                continue;
            }

            cycle[r] = true;
            cycle_nodes.emplace_back(r);
            for (int u = rep[rdsu.find(chosen_edge[r].first)]; u != r; u = rep[rdsu.find(chosen_edge[u].first)]) {
                cycle[u] = true;
                cycle_nodes.emplace_back(u);
            }

            for (int u : active)
                if (!cycle[u])
                    for (int i = 1; i < cycle_nodes.size(); i++) {
                        int t = cycle_nodes[i];
                        if (adj_matrix_transpose[r][u] > adj_matrix_transpose[t][u]) {
                            adj_matrix_transpose[r][u] = adj_matrix_transpose[t][u];
                            edge[r][u] = edge[t][u];
                        }
                        if (adj_matrix_transpose[u][r] > adj_matrix_transpose[u][t]) {
                            adj_matrix_transpose[u][r] = adj_matrix_transpose[u][t];
                            edge[u][r] = edge[u][t];
                        }
                    }

            int version = rdsu.record();
            vector<pair<int, int>> cycle_edges;
            for (int u : cycle_nodes) cycle_edges.emplace_back(chosen_edge[u]);
            history.emplace(version, r, cycle_edges);
            for (int i = 1; i < cycle_nodes.size(); i++) rdsu.unite(r, cycle_nodes[i]);
            rep[rdsu.find(r)] = r;

            active.erase(remove_if(active.begin(), active.end(), [&](int u) { return cycle[u] && u != r; }), active.end());
            while (!cycle_nodes.empty()) {
                cycle[cycle_nodes.back()] = false;
                cycle_nodes.pop_back();
            }
            chosen_edge[r] = {-1, -1};
            seen[r] = -1;
            v = r;
        }
    }

    vector<pair<int, int>> edges(n, {-1, -1});
    for (int v : active)
        if (v != root) edges[rdsu.find(chosen_edge[v].second)] = chosen_edge[v];

    while (!history.empty()) {
        auto [version, r, cycle_edges] = history.top();
        history.pop();

        auto e = edges[rdsu.find(r)];
        rdsu.rollback(version);
        for (auto f : cycle_edges) edges[rdsu.find(f.second)] = f;
        edges[rdsu.find(e.second)] = e;
    }

    vector<int> mst(n);
    for (int v = 0; v < n; v++) mst[v] = edges[v].first;
    mst[root] = root;
    return {len, mst};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto water = 0LL;
    vector<array<int, 5>> slides(n);
    vector<Point<int>> points(2 * n);
    for (int i = 0; i < n; i++) {
        auto &[xs, ys, xe, ye, w] = slides[i];
        cin >> xs >> ys >> xe >> ye >> w;

        water += w;
        points[2 * i] = {xs, ys};
        points[2 * i + 1] = {xe, ye};
    }
    points.emplace_back(0, 0);
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    auto index = [&](const Point<int> &p) {
        return lower_bound(points.begin(), points.end(), p) - points.begin();
    };
    int m = points.size();
    vector<vector<long long>> adj_list_transpose(m, vector<long long>(m, LLONG_MAX));
    for (int u = 0; u < m; u++)
        for (int v = 0; v < m; v++)
            if (u != v) adj_list_transpose[v][u] = manhattan_dist(points[u], points[v]);

    for (auto [xs, ys, xe, ye, w] : slides) {
        int u = index({xs, ys}), v = index({xe, ye});
        if (u != v) adj_list_transpose[v][u] = 0;
    }
    cout << water + edmonds_dense(m, adj_list_transpose, index({0, 0})).first;
}
