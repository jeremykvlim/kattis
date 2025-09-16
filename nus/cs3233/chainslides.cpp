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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, k;
    cin >> m >> k;

    vector<array<int, 3>> slides(m);
    for (auto &[v, x, y] : slides) {
        cin >> x >> y >> v;
        x--;
        y--;
    }
    sort(slides.begin(), slides.end());

    int n = 8, range = 1e9;
    vector<vector<int>> count(n, vector<int>(n, 0));
    for (int l = 0, r = 0, used = 0; r < m; r++) {
        count[slides[r][1]][slides[r][2]]++;
        used++;

        while (l <= r && used >= k) {
            vector<int> in(n, 0), out(n, 0);
            bool possible = true;
            for (int x = 0; x < n; x++) {
                for (int y = 0; y < n; y++) {
                    in[x] += count[y][x];
                    out[x] += count[x][y];
                }

                if (!in[x] || !out[x]) {
                    possible = false;
                    break;
                }
            }
            if (!possible) break;

            BoundedFlowNetwork<int, long long> bfn(2 * n);
            for (int x = 0; x < n; x++) {
                bfn.add_arc(x, x + n, 1, out[x], 0);
                for (int y = 0; y < n; y++)
                    if (count[x][y]) bfn.add_arc(x + n, y, 0, count[x][y], -1);
            }

            auto [c, feasible] = bfn.min_cost_b_flow();
            if (!feasible) break;

            if (-c >= k) {
                range = min(range, slides[r][0] - slides[l][0]);
                count[slides[l][1]][slides[l][2]]--;
                used--;
                l++;
            } else break;
        }
    }

    cout << (range == 1e9 ? -1 : range);
}
