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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
tuple<T, int, vector<int>> blossom(int n, const vector<vector<T>> &adj_matrix) {
    T w_max = 0;
    vector<vector<T>> w(2 * n + 2, vector<T>(2 * n + 2));
    vector<vector<int>> start(2 * n + 2, vector<int>(n + 1));
    vector<vector<pair<int, int>>> edge(2 * n + 2, vector<pair<int, int>>(2 * n + 2));
    for (int u = 1; u <= n; u++)
        for (int v = 1; v <= n; v++) {
            w_max = max(w_max, w[u][v] = adj_matrix[u][v]);
            start[u][v] = u == v ? u : 0;
            edge[u][v] = {u, v};
        }

    vector<T> potential(2 * n + 2);
    fill(potential.begin(), potential.begin() + n + 1, w_max);

    int nodes = n, count = 0;
    vector<int> match(2 * n + 2), base(2 * n + 2, 0), label(2 * n + 2), cheapest(2 * n + 2), parent(2 * n + 2), visited(2 * n + 2, 0);
    iota(base.begin(), base.begin() + n + 1, 0);
    vector<vector<int>> blossoms(2 * n + 2);
    auto augmenting_path = [&]() -> bool {
        fill(label.begin(), label.begin() + nodes + 1, -1);
        fill(cheapest.begin(), cheapest.begin() + nodes + 1, 0);

        queue<int> q;
        auto enqueue = [&](auto &&self, int x) -> void {
            if (x <= n) {
                q.emplace(x);
                return;
            }
            for (int y : blossoms[x]) self(self, y);
        };

        for (int x = 1; x <= nodes; x++)
            if (base[x] == x && !match[x]) {
                parent[x] = label[x] = 0;
                enqueue(enqueue, x);
            }
        if (q.empty()) return false;

        auto contract = [&](auto &&self, int b, int x) -> void {
            base[x] = b;
            if (x <= n) return;
            for (int a : blossoms[x]) self(self, b, a);
        };

        auto start_node = [&](int b, int x) -> pair<int, int> {
            int a = start[b][edge[b][x].first], p = find(blossoms[b].begin(), blossoms[b].end(), a) - blossoms[b].begin();
            if (p & 1) {
                reverse(blossoms[b].begin() + 1, blossoms[b].end());
                p = blossoms[b].size() - p;
            }
            return {a, p};
        };

        for (;;) {
            auto reduced_cost = [&](int x, int y) {
                auto [u, v] = edge[x][y];
                return potential[u] + potential[v] - 2 * w[x][y];
            };

            auto recompute = [&](int x) {
                cheapest[x] = 0;
                for (int v = 1; v <= n; v++)
                    if (w[v][x] && base[v] != x && !label[base[v]])
                        if (!cheapest[x] || reduced_cost(cheapest[x], x) > reduced_cost(v, x)) cheapest[x] = v;
            };

            auto process_edge = [&](const auto &e) {
                auto [u, v] = e;
                int x = base[u], y = base[v];
                if (!~label[y]) {
                    parent[y] = u;
                    label[y] = 1;
                    int z = base[match[y]];
                    cheapest[y] = cheapest[z] = label[z] = 0;
                    enqueue(enqueue, z);
                } else if (!label[y]) {
                    count++;
                    int lca = 0;
                    for (int a = x, b = y; a || b; swap(a, b)) {
                        if (!a) continue;
                        if (visited[a] == count) {
                            lca = a;
                            break;
                        }
                        visited[a] = count;
                        a = base[match[a]];
                        if (a) a = base[parent[a]];
                    }
                    if (!lca) {
                        auto augment = [&](int x, int y) {
                            auto rematch = [&](auto &&self, int x, int y) -> void {
                                match[x] = edge[x][y].second;
                                if (x <= n) return;

                                auto [a, p] = start_node(x, y);
                                for (int i = 0; i < p; i++) self(self, blossoms[x][i], blossoms[x][i ^ 1]);
                                self(self, a, y);
                                rotate(blossoms[x].begin(), blossoms[x].begin() + p, blossoms[x].end());
                            };

                            for (;;) {
                                int z = base[match[x]];
                                rematch(rematch, x, y);
                                if (!z) return;
                                rematch(rematch, z, base[parent[z]]);
                                x = base[parent[z]];
                                y = z;
                            }
                        };
                        augment(x, y);
                        augment(y, x);
                        return true;
                    }

                    int b = n + 1;
                    for (; b <= nodes && base[b]; b++);
                    if (b > nodes) nodes++;
                    potential[b] = label[b] = 0;
                    match[b] = match[lca];
                    blossoms[b] = {lca};
                    for (int a = x, z; a != lca; a = base[parent[z]]) {
                        blossoms[b].emplace_back(a);
                        blossoms[b].emplace_back(z = base[match[a]]);
                        enqueue(enqueue, z);
                    }
                    reverse(blossoms[b].begin() + 1, blossoms[b].end());
                    for (int a = y, z; a != lca; a = base[parent[z]]) {
                        blossoms[b].emplace_back(a);
                        blossoms[b].emplace_back(z = base[match[a]]);
                        enqueue(enqueue, z);
                    }
                    contract(contract, b, b);
                    for (int z = 1; z <= nodes; z++) {
                        if (z <= n) start[b][z] = 0;
                        w[b][z] = w[z][b] = 0;
                    }
                    for (int a : blossoms[b]) {
                        for (int z = 1; z <= nodes; z++)
                            if (!w[b][z] || reduced_cost(a, z) < reduced_cost(b, z)) {
                                w[b][z] = w[a][z];
                                edge[b][z] = edge[a][z];
                                w[z][b] = w[z][a];
                                edge[z][b] = edge[z][a];
                            }

                        for (int t = 1; t <= n; t++)
                            if (start[a][t]) start[b][t] = a;
                    }
                    recompute(b);
                }
                return false;
            };

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                if (label[base[u]] == 1) continue;

                for (int v = 1; v <= n; v++)
                    if (w[u][v] && base[u] != base[v]) {
                        if (!reduced_cost(u, v)) {
                            if (process_edge(edge[u][v])) return true;
                        } else {
                            int x = base[v];
                            if (reduced_cost(cheapest[x], x) > reduced_cost(u, x)) cheapest[x] = u;
                        }
                    }
            }

            T d = numeric_limits<T>::max() / 4;
            for (int b = n + 1; b <= nodes; b++)
                if (base[b] == b && label[b] == 1) d = min(d, potential[b] / 2);

            for (int x = 1; x <= nodes; x++)
                if (base[x] == x && cheapest[x]) {
                    if (!~label[x]) d = min(d, reduced_cost(cheapest[x], x));
                    else if (!label[x]) d = min(d, reduced_cost(cheapest[x], x) / 2);
                }

            for (int v = 1; v <= n; v++)
                if (!label[base[v]]) {
                    if (potential[v] <= d) return false;
                    potential[v] -= d;
                } else if (label[base[v]] == 1) potential[v] += d;

            for (int b = n + 1; b <= nodes; b++)
                if (base[b] == b) {
                    if (!label[base[b]]) potential[b] += 2 * d;
                    else if (label[base[b]] == 1) potential[b] -= 2 * d;
                }

            q = queue<int>();
            for (int x = 1; x <= nodes; x++)
                if (base[x] == x && cheapest[x] && base[cheapest[x]] != x && !reduced_cost(cheapest[x], x))
                    if (process_edge(edge[cheapest[x]][x])) return true;

            for (int b = n + 1; b <= nodes; b++)
                if (base[b] == b && label[b] == 1 && !potential[b]) {
                    for (int a : blossoms[b]) contract(contract, a, a);

                    auto [a, p] = start_node(b, parent[b]);
                    for (int i = 0; i < p; i += 2) {
                        int a1 = blossoms[b][i], a2 = blossoms[b][i + 1];
                        parent[a1] = edge[a2][a1].first;
                        label[a1] = 1;
                        label[a2] = cheapest[a1] = 0;
                        recompute(a2);
                        enqueue(enqueue, a2);
                    }
                    label[a] = 1;
                    parent[a] = parent[b];
                    for (int i = p + 1; i < blossoms[b].size(); i++) {
                        label[blossoms[b][i]] = -1;
                        recompute(blossoms[b][i]);
                    }
                    base[b] = 0;
                }
        }
    };
    int matches = 0;
    while (augmenting_path()) matches++;
    match.resize(n + 1);

    T weight = 0;
    for (int v = 1; v <= n; v++)
        if (match[v] && match[v] < v) weight += w[v][match[v]];
    return {weight, matches, match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Point<double> roost;
    int n;
    cin >> roost.x >> roost.y >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    double sum = 0, furthest = 0;
    vector<double> dist1(n);
    vector<vector<double>> dist2(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        furthest = max(furthest, dist1[i] = euclidean_dist(roost, points[i]));
        sum += dist1[i];
        for (int j = i + 1; j < n; j++) dist2[i][j] = euclidean_dist(points[i], points[j]);
    }

    vector<vector<int>> adj_matrix(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            auto d = dist1[i] + dist1[j] - dist2[i][j];
            if (d > 0) adj_matrix[i + 1][j + 1] = adj_matrix[j + 1][i + 1] = round(d * 1e5);
        }
    auto [weight, matches, match] = blossom(n, adj_matrix);

    auto total = 2 * sum - furthest;
    for (int i = 1; i <= n; i++) {
        int j = match[i];
        if (j && i < j) {
            auto d = dist1[i - 1] + dist1[j - 1] - dist2[i - 1][j - 1];
            if (d > 0) total -= d;
        }
    }
    cout << fixed << setprecision(6) << total;
}
