#include <bits/stdc++.h>
using namespace std;

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

    T max_flow(int s, int t) {
        if (s == t) return 0;

        vector<T> excess(n, 0);
        vector<int> height(n, 0), count(2 * n, 0);
        vector<typename vector<Arc>::iterator> curr(n);
        excess[t] = 1;
        height[s] = n;
        count[0] = n - 1;
        for (int v = 0; v < n; v++) curr[v] = network[v].begin();
        vector<stack<int>> active_stacks(2 * n);

        auto push = [&](int v, Arc &a, T delta) {
            int u = a.u;
            if (!excess[u] && delta) active_stacks[height[u]].emplace(u);
            a.cap -= delta;
            network[u][a.rev].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;
        };

        auto relabel = [&](int v, int h) {
            if (h < height[v] && height[v] < n) {
                height[v] = n + 1;
                count[height[v]]--;
            }
        };

        for (auto &&a : network[s]) push(s, a, a.cap);

        auto discharge = [&](int v, int &h) {
            while (excess[v] > 0)
                if (curr[v] == network[v].end()) {
                    height[v] = INT_MAX;

                    for (auto a = network[v].begin(); a != network[v].end(); a++)
                        if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                    count[height[v]]++;
                    if (!--count[h] && h < n)
                        for (int u = 0; u < n; u++) relabel(u, h);
                    h = height[v];
                } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                else curr[v]++;

            while (h >= 0 && active_stacks[h].empty()) h--;
        };

        if (!active_stacks[0].empty())
            for (int h = 0; h >= 0;) {
                int v = active_stacks[h].top();
                active_stacks[h].pop();

                discharge(v, h);
            }

        return -excess[s];
    }

    pair<T, U> max_flow_min_cost(int s, int t) {
        U cost = 0, bound = 0;
        int shift = __lg(n);
        for (int v = 0; v < n; v++)
            for (auto &&a : network[v]) {
                cost += a.cost * a.cap;
                a.cost <<= shift;
                bound = max(bound, a.cost);
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
            if (delta < inf) phi[v] -= delta + bound;
            else {
                phi[v] -= bound;
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

        while (bound > 1) {
            bound = max(bound >> shift, (U) 1);
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
                a.cost >>= shift;
                cost -= a.cost * a.cap;
            }

        return {flow, cost / 2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int m, n;
        cin >> m >> n;

        vector<string> grid(m);
        for (auto &row : grid) cin >> row;

        vector<vector<int>> h(m, vector<int>(n)), v(m, vector<int>(n));
        for (auto &row : h)
            for (int &cost : row) cin >> cost;
        for (auto &row : v)
            for (int &cost : row) cin >> cost;

        int count = 0;
        FlowNetwork<int, int> fn(3 * m * n + 2);
        vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] != '#') {
                    count++;
                    int x = i * n + j + 1;
                    if ((i + j) & 1) {
                        fn.add_arc(0, x, 2, 0);
                        fn.add_arc(x, x + m * n, 1, 0);
                        fn.add_arc(x, x + m * n, 1, h[i][j]);
                        fn.add_arc(x, x + 2 * m * n, 1, 0);
                        fn.add_arc(x, x + 2 * m * n, 1, v[i][j]);

                        for (int k = 0; k < 4; k++) {
                            int r = i + dr[k], c = j + dc[k];
                            if (0 <= r && r < m && 0 <= c && c < n && grid[r][c] != '#') {
                                int y = r * n + c + 1;
                                if (k & 1) fn.add_arc(x + m * n, y + m * n, 1, 0);
                                else fn.add_arc(x + 2 * m * n, y + 2 * m * n, 1, 0);
                            }
                        }
                    } else {
                        fn.add_arc(x, 1 + 3 * m * n, 2, 0);
                        fn.add_arc(x + m * n, x, 1, 0);
                        fn.add_arc(x + m * n, x, 1, h[i][j]);
                        fn.add_arc(x + 2 * m * n, x, 1, 0);
                        fn.add_arc(x + 2 * m * n, x, 1, v[i][j]);
                    }
                }

        auto [f, c] = fn.max_flow_min_cost(0, 3 * m * n + 1);
        if (f < count) cout << "NO\n";
        else cout << "YES " << c << "\n";
    }
}
