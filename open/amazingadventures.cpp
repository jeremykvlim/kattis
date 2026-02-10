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

    T dfs(int v, int t, T flow) {
        if (v == t) return flow;

        for (; it[v] != network[v].end(); it[v]++) {
            auto &[u, rev, cap, _] = *it[v];
            if (cap > 0 && dist[u] == dist[v] + 1) {
                T f = dfs(u, t, min(flow, cap));
                if (f > 0) {
                    cap -= f;
                    network[u][rev].cap += f;
                    return f;
                }
            }
        }
        return (T) 0;
    }

    T max_flow(int s, int t) {
        T flow = 0, f;
        while (bfs(s, t)) {
            for (int v = 0; v < n; v++) it[v] = network[v].begin();
            while ((f = dfs(s, t, numeric_limits<T>::max())) > 0) flow += f;
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

        auto reduced_cost = [&](int v, const Arc &a) {
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

                U c = reduced_cost(v, a);
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

                if (reduced_cost(v, *a) < 0) {
                    if (check(a->u)) {
                        a--;
                        continue;
                    }

                    push(v, *a, excess[v], true);
                    if (!excess[v]) return;
                } else delta = min(delta, reduced_cost(v, *a));
            }

            relabel(v, delta);
            active_stack.emplace_front(v);
        };

        while (epsilon > 1) {
            epsilon >>= 1;
            active_stack.clear();

            for (int v = 0; v < n; v++)
                for (auto &&a : network[v])
                    if (reduced_cost(v, a) < 0 && a.cap > 0) push(v, a, a.cap, false);

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

    int n, m;
    while (cin >> n >> m && n && m) {
        int rb, cb, rc, cc, rg, cg, ru, cu;
        cin >> rb >> cb >> rc >> cc >> rg >> cg >> ru >> cu;
        int cells = n * m;

        auto index = [&](int r, int c) {
            return (r - 1) * m + (c - 1);
        };

        auto cell = [&](int i) -> pair<int, int> {
            return {i / m + 1, i % m + 1};
        };

        FlowNetwork<int, long long> fn(2 * cells + 2);
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= m; c++) {
                if (r == ru && c == cu) continue;
                int i = index(r, c);
                fn.add_arc(2 * i, 2 * i + 1, (r == rc && c == cc ? 2 : 1), 0);
            }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= m; c++) {
                if (r == ru && c == cu) continue;
                int i = index(r, c);
                for (int k = 0; k < 4; k++) {
                    int x = r + dx[k], y = c + dy[k];
                    if (!(1 <= x && x <= n && 1 <= y && y <= m) || x == ru && y == cu) continue;
                    fn.add_arc(2 * i + 1, 2 * index(x, y), 2, 1);
                }
            }

        fn.add_arc(2 * cells, 2 * index(rc, cc), 2, 0);
        fn.add_arc(2 * index(rb, cb) + 1, 2 * cells + 1, 1, 0);
        fn.add_arc(2 * index(rg, cg) + 1, 2 * cells + 1, 1, 0);
        if (fn.min_cost_max_flow(2 * cells, 2 * cells + 1).first < 2) {
            cout << "NO\n";
            continue;
        }

        vector<vector<int>> adj_list(cells);
        for (int i = 0; i < cells; i++)
            for (auto [u, v, cap, cost] : fn.network[2 * i + 1]) {
                if (!(0 <= u && u < 2 * cells) || cost != 1 || u & 1) continue;
                if (fn.network[u][v].cap > 0) adj_list[i].emplace_back(u / 2);
            }

        auto travel = [&](int target) {
            vector<int> path;
            vector<bool> visited(cells, false);
            bool done = false;
            auto dfs = [&](auto &&self, int v) -> void {
                if (done) return;
                visited[v] = true;
                path.emplace_back(v);
                if (v == target) {
                    done = true;
                    return;
                }

                for (int u : adj_list[v])
                    if (!visited[u]) {
                        self(self, u);
                        if (done) return;
                    }
                path.pop_back();
            };
            dfs(dfs, index(rc, cc));
            return path;
        };
        auto path_b = travel(index(rb, cb));
        for (int i = 0; i + 1 < path_b.size(); i++) {
            int u = path_b[i], v = path_b[i + 1];
            for (int j = 0; j < adj_list[u].size(); j++)
                if (adj_list[u][j] == v) {
                    adj_list[u].erase(adj_list[u].begin() + j);
                    break;
                }
        }

        auto path_g = travel(index(rg, cg));
        if (path_b.empty() || path_b.back() != index(rb, cb) || path_g.empty() || path_g.back() != index(rg, cg)) {
            cout << "NO\n";
            continue;
        }

        auto convert = [&](auto &path) {
            string s;
            for (int i = 0; i + 1 < path.size(); i++) {
                auto [r1, c1] = cell(path[i]);
                auto [r2, c2] = cell(path[i + 1]);
                if (r1 + 1 == r2 && c1 == c2) s += 'U';
                else if (r1 - 1 == r2 && c1 == c2) s += 'D';
                else if (r1 == r2 && c1 + 1 == c2) s += 'R';
                else if (r1 == r2 && c1 - 1 == c2) s += 'L';
            }
            return s;
        };
        auto moves_g = convert(path_g), moves_b = convert(path_b);
        reverse(moves_b.begin(), moves_b.end());
        for (char &dir : moves_b)
            if (dir == 'U') dir = 'D';
            else if (dir == 'D') dir = 'U';
            else if (dir == 'L') dir = 'R';
            else dir = 'L';
        cout << "YES\n" << moves_b << moves_g << "\n";
    }
}
