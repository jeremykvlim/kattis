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

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
pair<vector<int>, double> travelling_salesman(const vector<Point<T>> &points, const vector<int> &starts) {
    int n = points.size();

    vector<vector<double>> dist(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) dist[i][j] = euclidean_dist(points[i], points[j]);

    auto nearest_neighbor = [&](int s) {
        vector<int> tour(n, -1);
        tour[0] = s;
        vector<bool> visited(n, false);
        visited[s] = true;
        for (int i = 1, v = 0; i < n; i++) {
            int u = -1;
            auto d = 1e9;
            for (int t = 0; t < n; t++)
                if (!visited[t] && d > dist[v][t]) {
                    d = dist[v][t];
                    u = t;
                }

            tour[i] = u;
            visited[u] = true;
            v = u;
        }

        return tour;
    };

    auto length = [&](const vector<int> &tour) {
        auto len = dist[tour[n - 1]][tour[0]];
        for (int i = 0; i < n - 1; i++) len += dist[tour[i]][tour[i + 1]];
        return len;
    };

    auto two_opt = [&](vector<int> &tour) {
        bool change;
        do {
            change = false;
            for (int i = 1; i < n - 1; i++)
                for (int j = i + 1; j < n; j++) {
                    int a = tour[i - 1], b = tour[i], c = tour[j], d = (j + 1 == n ? tour[0] : tour[j + 1]);
                    if (dist[a][c] + dist[b][d] < dist[a][b] + dist[c][d] - 1e-5) {
                        reverse(tour.begin() + i, tour.begin() + j + 1);
                        change = true;
                    }
                }
        } while (change);
    };

    mt19937 rng(random_device{}());
    vector<int> tour;
    double len;
    bool change;

    auto heuristic = [&]() -> bool {
        return change;
    };

    for (int s : starts) {
        tour = nearest_neighbor(s);
        two_opt(tour);

        len = length(tour);
        while (heuristic()) {
            change = false;
            auto t = tour;
            int i = -1, j = -1;
            while (i == j) {
                i = rng() % (n - 1) + 1;
                j = rng() % (n - 1) + 1;
            }
            if (i > j) swap(i, j);
            reverse(t.begin() + i, t.begin() + j + 1);
            two_opt(t);

            auto l = length(t);
            if (len > l) {
                len = l;
                tour = t;
                change = true;
            }
        }
    }
    if (n > 20) return {tour, len};

    vector<double> min1(n, 1e20), min2(n, 1e20);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j) {
                if (min1[i] > dist[i][j]) {
                    min2[i] = min1[i];
                    min1[i] = dist[i][j];
                } else min2[i] = min(min2[i], dist[i][j]);
            }
    if (2 * len < accumulate(min1.begin(), min1.end(), 0) + accumulate(min2.begin(), min2.end(), 0) + 1e-5) return {tour, len};

    vector<vector<int>> adj_list(n);
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++)
            if (u != v) adj_list[u].emplace_back(v);
        sort(adj_list[u].begin(), adj_list[u].end(), [&](int v, int t) { return dist[u][v] < dist[u][t]; });
    }

    vector<int> t{0};
    vector<vector<double>> memo(1 << n, vector<double>(n, 1e20));
    auto dfs = [&](auto &&self, int mask = 1, int v = 0, double cost = 0) {
        if (memo[mask][v] <= cost || len <= cost) return;
        memo[mask][v] = cost;

        if (mask == (1 << n) - 1) {
            auto l = cost + dist[v][0];
            if (len > l) {
                len = l;
                tour = t;
            }
            return;
        }

        vector<int> unvisited;
        for (int u = 0; u < n; u++)
            if (!((mask >> u) & 1)) unvisited.emplace_back(u);

        auto next = 1e20, back = 1e20;
        for (int u : unvisited) {
            next = min(next, dist[v][u]);
            back = min(back, dist[u][0]);
        }

        auto prim = [&]() -> double {
            int m = unvisited.size();
            if (m <= 1) return 0;

            vector<bool> visited(m, false);
            vector<double> d(m, 1e18);
            d[0] = 0;
            double weight = 0;
            for (int _ = 0; _ < m; _++) {
                int u = -1;
                for (int i = 0; i < m; i++)
                    if (!visited[i] && (u < 0 || d[i] < d[u])) u = i;
                visited[u] = true;
                weight += d[u];
                for (int v = 0; v < m; v++)
                    if (!visited[v]) d[v] = min(d[v], dist[unvisited[u]][unvisited[v]]);
            }
            return weight;
        };
        if (cost + prim() + next + back >= len) return;

        for (int u : adj_list[v])
            if (!((mask >> u) & 1)) {
                t.emplace_back(u);
                self(self, mask | (1 << u), u, cost + dist[v][u]);
                t.pop_back();
            }
    };
    dfs(dfs);
    return {tour, len};
}

template <typename T, typename U>
struct FlowNetwork {
    struct Arc {
        int u, rev;
        T cap;
        U cost;
        Arc(int u, int rev, T cap, U cost) : u(u), rev(rev), cap(cap), cost(cost) {}
    };

    int n;
    vector<vector<Arc>> network;
    vector<int> dist;
    vector<typename vector<Arc>::iterator> it;
    U inf;
    FlowNetwork(int n) : n(n), network(n), inf(numeric_limits<U>::max()), dist(n), it(n) {}

    void add_arc(int u, int v, T cap_uv, U cost, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv, cost);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu, -cost);
    }

    bool bfs(int s, int t) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, _, cap, __] : network[v])
                if (cap > 0 && !~dist[u]) {
                    dist[u] = dist[v] + 1;
                    q.emplace(u);
                }
        }
        return ~dist[t];
    }

    T dfs(int v, int t, T f) {
        if (v == t) return f;

        for (; it[v] != network[v].end(); it[v]++) {
            auto &[u, rev, cap, _] = *it[v];
            if (cap > 0 && dist[u] == dist[v] + 1) {
                T aug = dfs(u, t, min(f, cap));
                if (aug > 0) {
                    cap -= aug;
                    network[u][rev].cap += aug;
                    return aug;
                }
            }
        }
        return 0;
    }

    T max_flow(int s, int t) {
        T flow = 0, aug;
        while (bfs(s, t)) {
            for (int v = 0; v < n; v++) it[v] = network[v].begin();
            while ((aug = dfs(s, t, numeric_limits<T>::max())) > 0) flow += aug;
        }
        return flow;
    }

    pair<T, U> min_cost_max_flow(int s, int t) {
        U cost = 0, epsilon = 0;
        int scale = bit_ceil(2U * n);
        for (int v = 0; v < n; v++)
            for (auto &&a : network[v]) {
                cost += a.cost * a.cap;
                a.cost *= scale;
                epsilon = max(epsilon, abs(a.cost));
            }

        T flow = max_flow(s, t);

        vector<U> phi(n, 0), excess(n, 0);
        vector<int> count(n, 0);
        deque<int> active_stack;

        auto push = [&](int v, Arc &a, U delta, bool active) {
            if (delta > a.cap) delta = a.cap;
            int u = a.u;
            a.cap -= delta;
            network[u][a.rev].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;

            if (active && 0 < excess[u] && excess[u] <= delta) active_stack.emplace_front(u);
        };

        auto relabel = [&](int v, U delta) {
            if (delta < inf) phi[v] -= delta + epsilon;
            else {
                phi[v] -= epsilon;
                count[v]--;
            }
        };

        auto scaled_cost = [&](int v, const Arc &a) {
            int diff = count[v] - count[a.u];
            if (diff > 0) return inf;
            if (diff < 0) return -inf;
            return a.cost + phi[v] - phi[a.u];
        };

        auto check = [&](int v) {
            if (excess[v]) return false;

            U delta = inf;
            for (auto &&a : network[v]) {
                if (a.cap <= 0) continue;

                U c = scaled_cost(v, a);
                if (c < 0) return false;
                delta = min(delta, c);
            }

            relabel(v, delta);
            return true;
        };

        auto discharge = [&](int v) {
            U delta = inf;

            for (auto a = network[v].begin(); a != network[v].end(); a++) {
                if (a->cap <= 0) continue;

                if (scaled_cost(v, *a) < 0) {
                    if (check(a->u)) {
                        a--;
                        continue;
                    }

                    push(v, *a, excess[v], true);
                    if (!excess[v]) return;
                } else delta = min(delta, scaled_cost(v, *a));
            }

            relabel(v, delta);
            active_stack.emplace_front(v);
        };

        while (epsilon > 1) {
            epsilon >>= 1;
            active_stack.clear();

            for (int v = 0; v < n; v++)
                for (auto &&a : network[v])
                    if (scaled_cost(v, a) < 0 && a.cap > 0) push(v, a, a.cap, false);

            for (int v = 0; v < n; v++)
                if (excess[v] > 0) active_stack.emplace_front(v);

            while (!active_stack.empty()) {
                int v = active_stack.front();
                active_stack.pop_front();

                discharge(v);
            }
        }

        for (int v = 0; v < n; v++)
            for (auto &&a : network[v]) {
                a.cost /= scale;
                cost -= a.cost * a.cap;
            }

        return {flow, cost / 2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d;
    cin >> d;

    double prior = 0;
    vector<vector<Point<double>>> districts(d);
    for (int i = 0; i < d; i++) {
        int n;
        cin >> n;

        districts[i].resize(n);
        for (auto &[x, y] : districts[i]) cin >> x >> y;

        vector<int> s(n);
        iota(s.begin(), s.end(), 0);
        prior += travelling_salesman(districts[i], s).second;
    }

    FlowNetwork<long long, long long> fn(d + 2);
    for (int i = 0; i < d / 2; i++)
        for (int j = d / 2; j < d; j++) {
            vector<Point<double>> points;
            for (auto p : districts[i]) points.emplace_back(p);
            for (auto p : districts[j]) points.emplace_back(p);

            vector<int> s(points.size());
            iota(s.begin(), s.end(), 0);
            fn.add_arc(i, j, 1, llround(travelling_salesman(points, s).second * 1e6));
        }

    for (int i = 0; i < d; i++)
        if (i < d / 2) fn.add_arc(d, i, 1, 0);
        else fn.add_arc(i, d + 1, 1, 0);

    cout << fixed << setprecision(2) << prior << " " << fn.min_cost_max_flow(d, d + 1).second * 1e-6;
}
