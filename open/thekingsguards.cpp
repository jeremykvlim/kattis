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

    int n, m;
    vector<Arc> arcs;
    vector<T> balance;
    vector<U> potential;
    vector<pair<int, int>> parent;
    vector<int> depth, next, prev;
    U lb_offset;

    BoundedFlowNetwork(int n) : n(n), lb_offset(0), balance(n + 1, 0), potential(n + 1, 0), parent(n + 1, {-1, -1}),
                                depth(n + 1, 1), next(2 * (n + 1), 0), prev(2 * (n + 1), 0) {}

    void add_supply(int v, T b) {
        balance[v] += b;
    }

    void add_demand(int v, T b) {
        balance[v] -= b;
    }

    int add_arc(int u, int v, T lb, T ub, U cost = 0) {
        arcs.emplace_back(u, v, ub - lb, cost);
        arcs.emplace_back(v, u, 0, -cost);
        lb_offset += lb * cost;
        add_supply(v, lb);
        add_demand(u, lb);
        return arcs.size() - 2;
    }

    void connect(int u, int v) {
        next[u] = v;
        prev[v] = u;
    }

    int build_spanning_tree(int s = -1, int t = -1) {
        m = arcs.size();
        U penalty = 1;
        for (int e = 0; e < m; e += 2) penalty += abs(arcs[e].cost);

        connect(n << 1, n << 1 | 1);
        connect(n << 1 | 1, n << 1);
        for (int i = 0; i < n; i++) {
            int u = n, v = i;
            T b = balance[i];
            if (b < 0) {
                b = -b;
                swap(u, v);
            }
            int e = add_arc(u, v, 0, b, -penalty);
            e ^= arcs[e].u != i;
            parent[i] = {n, e};
            potential[i] = potential[n] - arcs[e].cost;
            connect(i << 1, i << 1 | 1);
            connect(i << 1 | 1, next[n << 1]);
            connect(n << 1, i << 1);
        }

        if (~s && ~t) return add_arc(t, s, 0, numeric_limits<T>::max() >> 2, -penalty) ^ 1;
        return -1;
    }

    void network_simplex() {
        auto reduced_cost = [&](int e) {
            auto [u, v, cap, cost] = arcs[e];
            return cost + potential[u] - potential[v];
        };

        depth[n] = 0;
        auto pivot = [&](int in) {
            auto [u, v, cap, cost] = arcs[in];
            U phi = cost + potential[u] - potential[v];

            T flow = arcs[in].cap;
            int out = in, dir = -1, b = -1;
            auto walk = [&](int x, int y) {
                auto step = [&](int &a, int d, int steps = 1) {
                    for (; steps; steps--, a = parent[a].first) {
                        int e = parent[a].second;
                        T f = arcs[e ^ !d].cap;
                        if (make_pair(flow, out) > make_pair(f, e)) {
                            tie(flow, out) = tie(f, e);
                            dir = d;
                            b = a;
                        }
                    }
                };
                if (depth[x] >= depth[y]) step(x, 0, depth[x] - depth[y]);
                else step(y, 1, depth[y] - depth[x]);

                while (x != y) {
                    step(x, 0);
                    step(y, 1);
                }
                return x;
            };
            int lca = walk(u, v);
            arcs[in].cap -= flow;
            arcs[in ^ 1].cap += flow;
            auto augment = [&](int a, int d) {
                for (; a != lca; a = parent[a].first) {
                    int e = parent[a].second;
                    arcs[e ^ !d].cap -= flow;
                    arcs[e ^ d].cap += flow;
                }
            };
            augment(u, 0);
            augment(v, 1);

            if (in == out || dir == -1 || b == -1) return;

            auto basis_exchange = [&](int a, int p, int in) {
                auto update = [&](int a, int p) {
                    for (int t = a, d = depth[p >> 1]; t != a + 1; t = next[t])
                        if (!(t & 1)) {
                            potential[t >> 1] += phi;
                            depth[t >> 1] = ++d;
                        } else d--;

                    connect(prev[a], next[a + 1]);
                    connect(a + 1, next[p]);
                    connect(p, a);
                };

                do {
                    update(a << 1, p << 1);
                    if (a == b) break;
                    auto [t, e] = parent[a];
                    parent[a] = {p, in};
                    p = exchange(a, t);
                    in = e ^ 1;
                } while (true);
                parent[b] = {p, in};
            };

            if (!dir) {
                phi = -phi;
                basis_exchange(u, v, in);
            } else basis_exchange(v, u, in ^ 1);
        };

        auto basis_edge = [&](int e) {
            auto [u, v, cap, cost] = arcs[e];
            int a = parent[u].second;
            if (~a && (a >> 1) == (e >> 1)) return true;
            int b = parent[v].second;
            if (~b && (b >> 1) == (e >> 1)) return true;
            return false;
        };

        list<int> candidates;
        for (int arc = 0, aug = arcs.size(), size = max(64, (int) sqrt(aug / 2)), len = size / 4, k = len / 10, count = 0;;) {
            for (int _ = 0; _ < k && !candidates.empty(); _++) {
                U cost = 0;
                int in = -1;
                for (auto it = candidates.begin(); it != candidates.end();) {
                    int e = *it;
                    U c = 0;
                    if (arcs[e].cap <= 0 || basis_edge(e) || (c = reduced_cost(e)) >= 0) {
                        it = candidates.erase(it);
                        continue;
                    }
                    if (make_pair(cost, in) > make_pair(c, e)) tie(cost, in) = tie(c, e);
                    it++;
                }
                if (!~in) break;
                pivot(in);
                count = 0;
            }
            candidates.clear();

            U cost = 0;
            int in = -1;
            for (int block = count + size; count < block; count++, ++arc %= aug)
                if (!basis_edge(arc) && arcs[arc].cap > 0) {
                    U c = reduced_cost(arc);
                    if (c < 0) {
                        if (candidates.size() < len) candidates.emplace_back(arc);
                        if (make_pair(cost, in) > make_pair(c, arc)) tie(cost, in) = tie(c, arc);
                    }
                }

            if (candidates.empty()) {
                if (count >= aug) break;
                continue;
            }
            pivot(in);
            count = 0;
        }
    }

    bool feasible() {
        for (int i = 0; i < n; i++)
            if (arcs[(i << 1) + m].cap) return false;
        return true;
    }

    U min_cost() {
        U cost = lb_offset;
        for (int e = 0; e < m; e += 2) cost += arcs[e].cost * arcs[e ^ 1].cap;
        return cost;
    }

    tuple<T, U, bool> min_cost_max_flow(int s, int t) {
        int e = build_spanning_tree(s, t);
        network_simplex();
        if (!feasible()) return {(U) 0, (T) 0, false};
        return {arcs[e].cap, min_cost(), true};
    }

    pair<U, bool> min_cost_b_flow() {
        build_spanning_tree();
        network_simplex();
        if (!feasible()) return {(U) 0, false};
        return {min_cost(), true};
    }
};

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
struct ReachabilityTree {
    int n;
    vector<int> parent;
    vector<vector<int>> adj_list;
    vector<int> weight;

    ReachabilityTree(int m, vector<tuple<int, int, T>> &edges) : n(m), parent(2 * m), adj_list(2 * m), weight(2 * m, 0) {
        sort(edges.begin(), edges.end(), [&](auto e1, auto e2) { return get<2>(e1) < get<2>(e2); });
        DisjointSets dsu(2 * n);
        for (auto [u, v, w] : edges) {
            int u_set = dsu.find(u), v_set = dsu.find(v);
            if (u_set != v_set) {
                n++;
                weight[n] = w;
                parent[u_set] = parent[v_set] = dsu.sets[u_set] = dsu.sets[v_set] = dsu.sets[n] = n;
                adj_list[n].emplace_back(u_set);
                adj_list[n].emplace_back(v_set);
            }
        }
    }

    vector<int> post_order_traversal() {
        vector<int> order;
        auto dfs = [&](auto &&self, int v) -> void {
            for (int u : adj_list[v]) self(self, u);
            order.emplace_back(v);
        };
        dfs(dfs, n);
        return order;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, R, g;
    cin >> n >> R >> g;

    int sum = 0;
    vector<tuple<int, int, int>> edges(R);
    for (auto &[a, b, c] : edges) {
        cin >> a >> b >> c;

        sum += c;
    }
    for (int i = 2; i <= n; i++) edges.emplace_back(1, i, sum + 1);

    ReachabilityTree rt(n, edges);
    vector<int> dp(rt.n + 1, 0);
    for (int v : rt.post_order_traversal()) {
        if (rt.adj_list[v].empty()) dp[v] = 1;
        else
            for (int u : rt.adj_list[v]) dp[v] += dp[u];
    }

    BoundedFlowNetwork<int, int> bfn(rt.n + g + 1);
    for (int i = 0; i < g; i++) {
        int k;
        cin >> k;

        while (k--) {
            int v;
            cin >> v;

            bfn.add_arc(rt.n + i, v - 1, 0, 1, 0);
        }
        bfn.add_arc(rt.n + g, rt.n + i, 0, 1, 0);
    }

    int delta = 0;
    for (int v = 1; v <= rt.n; v++)
        if (rt.parent[v]) {
            int d = rt.weight[rt.parent[v]] - rt.weight[v];
            delta += d;
            bfn.add_arc(v - 1, rt.parent[v] - 1, 0, 1, -d);
            if (v > n) bfn.add_arc(v - 1, rt.parent[v] - 1, 0, dp[v] - 1, 0);
        }
    bfn.add_supply(rt.n + g, g);
    bfn.add_demand(rt.n - 1, g);

    auto [cost, feasible] = bfn.min_cost_b_flow();
    if (!feasible || cost + delta > sum) cout << -1;
    else cout << cost + delta;
}
