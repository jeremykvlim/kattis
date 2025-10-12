#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if (!is_floating_point_v<T>) return (v > 0) - (v < 0);
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
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int _, n, m;
    cin >> _ >> n >> m;

    int k = 4 * n;
    vector<int> degree(k);
    vector<vector<int>> adj_list(k);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
        degree[a - 1]++;
        degree[b - 1]++;
    }
    for (auto &neighbors : adj_list) sort(neighbors.begin(), neighbors.end(), [&](int u, int v) { return degree[u] < degree[v]; });

    vector<int> indices(k);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int u, int v) { return degree[u] < degree[v]; });

    vector<int> order;
    vector<bool> visited(k, false);
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        order.emplace_back(v);
        for (int u : adj_list[v])
            if (!visited[u]) self(self, u);
    };
    for (int v : indices)
        if (!visited[v]) dfs(dfs, v);

    vector<int> row(k, -1), col(k, -1);
    vector<vector<int>> grid(n, vector<int>(4));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < 4; c++) {
            int v = !(r & 1) ? order[4 * r + c] : order[4 * r + 3 - c];
            grid[r][c] = v;
            row[v] = r;
            col[v] = c;
        }

    auto sweep = [&](bool dir) {
        for (int r1 = !dir ? 0 : n - 1; (!dir ? r1 < n : ~r1); (!dir ? r1++ : r1--))
            for (int c1 = 0; c1 < 4; c1++)
                for (int r2 = max(0, r1 - 2); r2 < min(n, r1 + 2); r2++)
                    for (int c2 = 0; c2 < 4; c2++) {
                        int u = grid[r1][c1], v = grid[r2][c2];
                        if (u != v && (degree[u] || degree[v])) {
                            auto happiness = 0.;

                            for (int t : adj_list[u])
                                if (v != t) {
                                    int r3 = row[t], c3 = col[t];
                                    happiness += 1. / squared_dist(Point{r1, c1}, Point{r3, c3}) - 1. / squared_dist(Point{r2, c2}, Point{r3, c3});
                                }

                            for (int t : adj_list[v])
                                if (u != t) {
                                    int r3 = row[t], c3 = col[t];
                                    happiness -= 1. / squared_dist(Point{r1, c1}, Point{r3, c3}) - 1. / squared_dist(Point{r2, c2}, Point{r3, c3});
                                }

                            if (sgn(happiness) == -1) {
                                swap(grid[r1][c1], grid[r2][c2]);
                                swap(row[u], row[v]);
                                swap(col[u], col[v]);
                            }
                        }
                    }
    };
    sweep(false);
    sweep(true);

    for (auto r : grid) {
        for (int v : r) cout << v + 1 << " ";
        cout << "\n";
    }
}
