#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
struct BoundedFlowNetwork {
    struct Arc {
        int u, v;
        T cap;
        U cost;
        Arc(int u, int v, T cap, U cost) : u(u), v(v), cap(cap), cost(cost) {}
    };

    int n;
    vector<vector<Arc>> network;
    vector<T> balance;
    U cost;

    BoundedFlowNetwork(int n) : n(n), network(n + 2), balance(n + 2, 0), cost(0) {}

    void add_supply(int v, T b) {
        balance[v] += b;
    }

    void add_demand(int v, T b) {
        balance[v] -= b;
    }

    pair<int, int> add_arc(int u, int v, T lb, T ub, U c = 0) {
        int su = network[u].size(), sv = network[v].size();
        T f = c < 0 ? ub : lb;
        cost += c * f;
        add_supply(v, f);
        add_demand(u, f);
        if (u == v) return {su, sv};

        if (c < 0) {
            network[v].emplace_back(u, network[u].size(), ub - lb, -c);
            network[u].emplace_back(v, network[v].size() - 1, 0, c);
        } else {
            network[u].emplace_back(v, network[v].size(), ub - lb, c);
            network[v].emplace_back(u, network[u].size() - 1, 0, -c);
        }
        return {su, sv};
    }

    void successive_shortest_path() {
        for (int v = 0; v < n; v++)
            if (balance[v] > 0) add_arc(n, v, 0, balance[v]);
            else if (balance[v] < 0) add_arc(v, n + 1, 0, -balance[v]);

        U inf = numeric_limits<U>::max();
        vector<U> potential(n + 2, 0), dist(n + 2);
        vector<pair<int, int>> prev(n + 2, {-1, -1});
        priority_queue<pair<U, int>, vector<pair<U, int>>, greater<>> pq;

        for (;;) {
            fill(dist.begin(), dist.end(), inf);
            dist[n] = 0;
            pq.emplace(0, n);

            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (dist[v] != d) continue;

                for (int i = 0; i < network[v].size(); i++) {
                    auto &[u, _, cap, c] = network[v][i];
                    U phi = c + potential[v] - potential[u];
                    if (cap > 0 && dist[u] > d + phi) {
                        dist[u] = d + phi;
                        pq.emplace(d + phi, u);
                        prev[u] = {v, i};
                    }
                }
            }

            if (dist[n + 1] == inf) break;

            for (int v = 0; v < n + 2; v++)
                if (dist[v] != inf) potential[v] += dist[v];

            T f = numeric_limits<T>::max();
            for (int v = n + 1; v != n; v = prev[v].first) {
                auto [u, e] = prev[v];
                f = min(f, network[u][e].cap);
            }

            cost += (potential[n + 1] - potential[n]) * f;
            for (int v = n + 1; v != n; v = prev[v].first) {
                auto [u, e] = prev[v];
                auto &[w, rev, cap, c] = network[u][e];
                cap -= f;
                network[v][rev].cap += f;
            }
        }
    }

    bool feasible() {
        if (accumulate(balance.begin(), balance.end(), (T) 0)) return false;
        return all_of(network[n].begin(), network[n].end(), [](auto &a) { return !a.cap; });
    }

    tuple<T, U, bool> min_cost_max_flow(int s, int t) {
        U penalty = 1;
        for (int v = 0; v < n; v++)
            for (auto &a : network[v])
                if (a.cost > 0) penalty += a.cost;

        T cap = -balance[s];
        for (auto &a : network[s]) cap += a.cap;

        auto [st, ss] = add_arc(t, s, 0, max(cap, (T) 0), -penalty);
        successive_shortest_path();
        if (!feasible()) return {0, 0, false};
        return {network[s][ss].cap, cost + penalty * network[s][ss].cap, true};
    }

    pair<U, bool> min_cost_b_flow() {
        successive_shortest_path();
        if (!feasible()) return {0, false};
        return {cost, true};
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

        BoundedFlowNetwork<int, long long> bfn(2 * cells + 2);

        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= m; c++)
                if (r != ru || c != cu) {
                    int i = index(r, c);
                    bfn.add_arc(2 * i, 2 * i + 1, 0, (r == rc && c == cc) ? 2 : 1, 0);
                }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        for (int r = 1; r <= n; r++)
            for (int c = 1; c <= m; c++)
                if (r != ru || c != cu) {
                    int i = index(r, c);
                    for (int k = 0; k < 4; k++) {
                        int x = r + dx[k], y = c + dy[k];
                        if (1 <= x && x <= n && 1 <= y && y <= m && (x != ru || y != cu)) bfn.add_arc(2 * i + 1, 2 * index(x, y), 0, 2, 1);
                    }
                }
        bfn.add_arc(2 * cells, 2 * index(rc, cc), 0, 2, 0);
        bfn.add_arc(2 * index(rb, cb) + 1, 2 * cells + 1, 0, 1, 0);
        bfn.add_arc(2 * index(rg, cg) + 1, 2 * cells + 1, 0, 1, 0);
        bfn.add_supply(2 * cells, 2);
        bfn.add_demand(2 * cells + 1, 2);
        if (!bfn.min_cost_b_flow().second) {
            cout << "NO\n";
            continue;
        }

        vector<vector<int>> adj_list(cells);
        for (int u = 1; u < 2 * cells; u += 2)
            for (auto &[v, rev, cap, cost] : bfn.network[u])
                if (v < 2 * cells && !(v & 1) && cost == 1 && bfn.network[v][rev].cap > 0) adj_list[u / 2].emplace_back(v / 2);

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