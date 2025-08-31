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
    U inf;
    FlowNetwork(int n) : n(n), network(n), inf(numeric_limits<U>::max()) {}

    void add_arc(int u, int v, T cap_uv, U cost, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv, cost);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu, -cost);
    }

    pair<T, vector<U>> min_cost_max_flow(int s, int t) {
        T flow = 0;
        vector<U> costs{0};
        for (vector<U> dist(n, inf), phi(n, 0);; fill(dist.begin(), dist.end(), inf)) {
            vector<pair<int, int>> prev(n, {-1, -1});
            dist[s] = 0;
            priority_queue<pair<U, int>, vector<pair<U, int>>, greater<>> pq;
            pq.emplace(0, s);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (dist[v] != d) continue;

                for (int i = 0; i < network[v].size(); i++) {
                    auto [u, rev, cap, cost] = network[v][i];
                    if (cap > 0 && dist[u] > dist[v] + cost + phi[v] - phi[u]) {
                        dist[u] = dist[v] + cost + phi[v] - phi[u];
                        prev[u] = {v, i};
                        pq.emplace(dist[u], u);
                    }
                }
            }
            if (dist[t] == inf) break;

            for (int v = 0; v < n; v++)
                if (dist[v] < inf) phi[v] += dist[v];

            T f = numeric_limits<T>::max();
            for (int v = t; v != s; v = prev[v].first) f = min(f, network[prev[v].first][prev[v].second].cap);

            flow += f;
            costs.emplace_back(costs.back() + f * phi[t]);

            for (int v = t; v != s; v = prev[v].first) {
                auto &[u, rev, cap, cost] = network[prev[v].first][prev[v].second];
                cap -= f;
                network[v][rev].cap += f;
            }
        }

        return {flow, costs};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w, h, c, t;
    cin >> w >> h >> c >> t;

    vector<string> grid(h);
    for (auto &row : grid) cin >> row;

    Point<int> hq;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if (grid[i][j] == 'K') {
                hq = {i, j};
                goto found;
            }
    found:;

    vector<Point<int>> tasks(t + 1);
    for (int i = 1; i <= t; i++) {
        cin >> tasks[i].y >> tasks[i].x;
        tasks[i] -= {1, 1};
    }

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    auto bfs = [&](Point<int> s) {
        vector<vector<int>> dist(h, vector<int>(w, 1e9));
        dist[s.x][s.y] = 0;
        queue<Point<int>> q;
        q.emplace(s);
        while (!q.empty()) {
            auto a = q.front();
            q.pop();

            for (int k = 0; k < 4; k++) {
                auto b = a + Point<int>{dx[k], dy[k]};
                if (!(0 <= b.x && b.x < h && 0 <= b.y && b.y < w) || grid[b.x][b.y] == '#') continue;
                if (dist[b.x][b.y] > dist[a.x][a.y] + 1) {
                    dist[b.x][b.y] = dist[a.x][a.y] + 1;
                    q.emplace(b);
                }
            }
        }
        return dist;
    };

    auto dist_hq = bfs(hq);
    vector<int> hq_to_task(t + 1, 1e9);
    for (int i = 1; i <= t; i++) {
        auto a = tasks[i];
        hq_to_task[i] = dist_hq[a.x][a.y];

        if (hq_to_task[i] >= 1e9) {
            cout << "impossible";
            exit(0);
        }
    }

    vector<vector<int>> task_to_task(t + 1, vector<int>(t + 1, 1e9));
    vector<int> task_to_hq(t + 1, 1e9);
    for (int i = 1; i <= t; i++) {
        auto dist_task = bfs(tasks[i]);
        task_to_hq[i] = dist_task[hq.x][hq.y];

        if (task_to_hq[i] >= 1e9) {
            cout << "impossible";
            exit(0);
        }

        for (int j = i + 1; j <= t; j++) {
            auto a = tasks[j];
            task_to_task[i][j] = dist_task[a.x][a.y];
        }
    }

    FlowNetwork<int, int> fn(2 * t + 2);
    for (int i = 1; i <= t; i++) {
        fn.add_arc(0, i, 1, 0);
        fn.add_arc(i + t, 2 * t + 1, 1, 0);
    }

    for (int i = 1; i <= t; i++)
        for (int j = i + 1; j <= t; j++)
            if (task_to_task[i][j] < 1e9) fn.add_arc(i, t + j, 1, task_to_task[i][j] - task_to_hq[i] - hq_to_task[j]);

    auto [f, costs] = fn.min_cost_max_flow(0, 2 * t + 1);

    if (f < max(t - c, 0)) {
        cout << "impossible";
        exit(0);
    }

    auto steps = 0LL;
    for (int i = 1; i <= t; i++) steps += hq_to_task[i] + task_to_hq[i];
    cout << steps + *min_element(costs.begin() + max(t - c, 0), costs.end());
}
