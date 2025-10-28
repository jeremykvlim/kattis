#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<pair<int, int>, int, Hash> min_edges;
    for (int i = 0; i < n; i++) {
        int j, t;
        cin >> j >> t;
        j--;

        auto p = minmax(i, j);
        if (min_edges.count(p)) min_edges[p] = min(min_edges[p], t);
        else min_edges[p] = t;
    }

    vector<int> degree(n);
    vector<vector<pair<int, int>>> adj_list_graph(n);
    vector<array<int, 3>> edges;
    for (auto [p, w] : min_edges) {
        auto [u, v] = p;
        degree[u]++;
        degree[v]++;
        adj_list_graph[u].emplace_back(v, w);
        adj_list_graph[v].emplace_back(u, w);
        edges.push_back({u, v, w});
    }

    int ccs = 0;
    vector<int> component(n, -1);
    auto dfs1 = [&](auto &&self, int v) -> void {
        for (auto [u, w] : adj_list_graph[v])
            if (!~component[u]) {
                component[u] = ccs;
                self(self, u);
            }
    };
    
    for (int v = 0; v < n; v++)
        if (!~component[v]) {
            component[v] = ccs;
            dfs1(dfs1, v);
            ccs++;
        }
    
    vector<vector<int>> members(ccs);
    for (int v = 0; v < n; v++) members[component[v]].emplace_back(v);

    queue<int> q;
    for (int v = 0; v < n; v++)
        if (degree[v] < 2) q.emplace(v);

    vector<bool> cycle(n, true);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (!cycle[v]) continue;

        cycle[v] = false;
        for (auto [u, w] : adj_list_graph[v])
            if (cycle[u])
                if (--degree[u] == 1) q.emplace(u);
    }

    vector<vector<pair<int, int>>> adj_list_tree(n), adj_list_ring(n);
    for (auto [u, v, w] : edges)
        if (cycle[u] && cycle[v]) {
            adj_list_ring[u].emplace_back(v, w);
            adj_list_ring[v].emplace_back(u, w);
        } else {
            adj_list_tree[u].emplace_back(v, w);
            adj_list_tree[v].emplace_back(u, w);
        }

    vector<int> parent(n, -1), root(n, -1);
    vector<long long> tree_dist(n, 0);
    auto dfs2 = [&](auto &&self, int v, int r) -> void {
        for (auto [u, w] : adj_list_tree[v])
            if (!~root[u]) {
                parent[u] = v;
                root[u] = r;
                tree_dist[u] = tree_dist[v] + w;
                self(self, u, r);
            }
    };

    for (int v = 0; v < n; v++)
        if (cycle[v] && !~root[v]) {
            root[v] = v;
            dfs2(dfs2, v, v);
        }

    for (int v = 0; v < n; v++)
        if (!~root[v]) {
            root[v] = v;
            dfs2(dfs2, v, v);
        }

    vector<vector<int>> adj_list(n + 1);
    for (int i = 0; i < n; i++)
        if (parent[i] != -1) adj_list[parent[i]].emplace_back(i);
        else adj_list[n].emplace_back(i);

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(n + 1), depth(n + 1, 0), anc_mask(n + 1, 0), head(n + 2);
    auto dfs = [&](auto &&self, int v, int prev) -> void {
        tour.emplace_back(v, prev);
        index[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
    };
    dfs(dfs, n, n);
    for (auto [v, p] : tour) anc_mask[v] = anc_mask[p] | lsb(index[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = index[u] ^ index[v]; above) {
            above = (anc_mask[u] & anc_mask[v]) & -bit_floor(above);
            if (unsigned below = anc_mask[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(index[u] & -below) | below];
            }
            if (unsigned below = anc_mask[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(index[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    vector<int> ring_indices(n, -1);
    vector<long long> ring_perimeter(ccs, 0);
    vector<vector<long long>> ring_pref(ccs);
    for (int c = 0; c < ccs; c++) {
        int s = -1;
        for (int v : members[c])
            if (cycle[v]) {
                s = v;
                break;
            }
        if (s == -1) continue;

        int v = s, u = -1;
        vector<int> order;
        vector<long long> weights;
        do {
            order.emplace_back(v);
            int i = -1;
            for (int j = 0; j < adj_list_ring[v].size(); j++)
                if (adj_list_ring[v][j].first != u) {
                    i = j;
                    break;
                }

            auto [t, w] = adj_list_ring[v][i];
            weights.emplace_back(w);
            ring_perimeter[c] += w;
            u = v;
            v = t;
        } while (v != s);

        int m = order.size();
        ring_pref[c].assign(m, 0);
        for (int i = 0; i < m; i++) {
            ring_indices[order[i]] = i;
            if (i) ring_pref[c][i] = ring_pref[c][i - 1] + weights[i - 1];
        }
    }

    int l;
    cin >> l;

    vector<int> houses;
    vector<bool> seen(n, false);
    vector<array<int, 3>> extra(l);
    for (auto &[x, y, t] : extra) {
        cin >> x >> y >> t;
        x--;
        y--;

        if (!seen[x]) {
            seen[x] = true;
            houses.emplace_back(x);
        }
        if (!seen[y]) {
            seen[y] = true;
            houses.emplace_back(y);
        }
    }

    vector<int> indices(n, -1);
    int m = houses.size();
    for (int i = 0; i < m; i++) indices[houses[i]] = i;

    vector<vector<int>> component_houses(ccs);
    for (int i = 0; i < m; i++) component_houses[component[houses[i]]].emplace_back(i);

    auto base_time = [&](int a, int b) -> long long {
        if (component[a] != component[b]) return 1e18;
        if (root[a] == root[b]) return tree_dist[a] + tree_dist[b] - 2 * tree_dist[lca(a, b)];
        else {
            int c = component[root[a]], i = ring_indices[root[a]], j = ring_indices[root[b]];
            auto dist = abs(ring_pref[c][i] - ring_pref[c][j]);
            return tree_dist[a] + tree_dist[b] + min(dist, ring_perimeter[c] - dist);
        }
    };

    vector<vector<long long>> dist(m, vector<long long>(m, 1e18));
    for (int i = 0; i < m; i++) {
        dist[i][i] = 0;
        int u = houses[i];
        for (int j = 0; j < m; j++)
            if (i != j) {
                int v = houses[j];
                if (component[u] == component[v]) dist[i][j] = min(dist[i][j], base_time(u, v));
            }
    }

    for (auto [x, y, t] : extra) {
        int i = indices[x], j = indices[y];
        if (i != -1 && j != -1)
            if (dist[i][j] > t) dist[i][j] = dist[j][i] = t;
    }

    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            if (dist[j][i] != 1e18)
                for (int k = 0; k < m; k++)
                    if (dist[i][k] != 1e18) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    int Q;
    cin >> Q;

    while (Q--) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        auto t = base_time(a, b);
        int ca = component[a], cb = component[b];
        if (!component_houses[ca].empty() && !component_houses[cb].empty()) {
            vector<long long> time_a(component_houses[ca].size(), 1e18), time_b(component_houses[cb].size(), 1e18);
            for (int i = 0; i < component_houses[ca].size(); i++) time_a[i] = base_time(a, houses[component_houses[ca][i]]);
            for (int j = 0; j < component_houses[cb].size(); j++) time_b[j] = base_time(houses[component_houses[cb][j]], b);
            for (int i = 0; i < component_houses[ca].size(); i++)
                if (time_a[i] != 1e18) {
                    int u = component_houses[ca][i];
                    for (int j = 0; j < component_houses[cb].size(); j++)
                        if (time_b[j] != 1e18) {
                            int v = component_houses[cb][j];
                            if (dist[u][v] != 1e18) t = min(t, time_a[i] + time_b[j] + dist[u][v]);
                        }
                }
        }
        if (t == 1e18) cout << "-1\n";
        else cout << t << "\n";
    }
}
