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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

template <typename T>
struct ReachabilityTree {
    int n;
    vector<int> parent;
    vector<vector<int>> adj_list;
    vector<T> weight;

    ReachabilityTree(int m, vector<tuple<int, int, T>> &edges) : n(m), parent(2 * m), adj_list(2 * m), weight(2 * m, 0) {
        DisjointSets dsu(2 * m);
        vector<int> rep(2 * m);
        iota(rep.begin(), rep.end(), 0);

        for (auto [u, v, w] : edges) {
            int u_set = dsu.find(u), v_set = dsu.find(v);
            if (u_set != v_set) {
                n++;
                weight[n] = w;
                parent[rep[u_set]] = parent[rep[v_set]] = n;
                adj_list[n].emplace_back(rep[u_set]);
                adj_list[n].emplace_back(rep[v_set]);
                auto [big, small] = dsu.unite(u_set, v_set);
                rep[big] = n;
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
    sort(edges.begin(), edges.end(), [&](auto e1, auto e2) { return get<2>(e1) < get<2>(e2); });

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
