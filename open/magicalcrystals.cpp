#include <bits/stdc++.h>
using namespace std;

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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<int, int, int>> edges(n - 1);
    for (auto &[a, b, w] : edges) cin >> a >> b >> w;

    ReachabilityTree rt(n, edges);
    int m = rt.n + 1;

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(m), depth(m, 0), in(m), anc_mask(m, 0), head(m + 1);
    int count = 0;
    auto dfs1 = [&](auto &&self, int v, int prev) -> void {
        tour.emplace_back(v, prev);
        index[v] = tour.size();
        in[v] = count++;

        for (int u : rt.adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
    };
    dfs1(dfs1, rt.n, rt.n);
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

    int q;
    cin >> q;

    vector<vector<int>> virtual_tree(m);
    vector<int> crystals(m, 0), dp(m, 0);
    vector<bool> seen(m, false);
    while (q--) {
        int ni;
        cin >> ni;

        vector<int> undo;
        while (ni--) {
            int x, v;
            cin >> x >> v;

            if (!seen[x]) {
                seen[x] = true;
                undo.emplace_back(x);
            }
            crystals[x] += v;
        }

        vector<int> cities;
        for (int x : undo)
            if (crystals[x]) cities.emplace_back(x);

        if (cities.empty()) {
            cout << "0\n";
            for (int x : undo) {
                seen[x] = false;
                crystals[x] = 0;
            }
            continue;
        }

        sort(cities.begin(), cities.end(), [&](int u, int v) { return in[u] < in[v]; });
        auto all = cities;
        for (int i = 0; i + 1 < cities.size(); i++) all.emplace_back(lca(cities[i], cities[i + 1]));
        sort(all.begin(), all.end(), [&](int u, int v) { return in[u] < in[v]; });
        all.erase(unique(all.begin(), all.end()), all.end());
        for (int i = 0; i + 1 < all.size(); i++) virtual_tree[lca(all[i], all[i + 1])].emplace_back(all[i + 1]);

        vector<int> order;
        auto dfs2 = [&](auto &&self, int u) -> void {
            for (int v : virtual_tree[u]) self(self, v);
            order.emplace_back(u);
        };
        dfs2(dfs2, all[0]);

        auto cost = 0LL;
        for (int v : order) {
            dp[v] = crystals[v];
            auto c = 0LL;
            for (int u : virtual_tree[v]) {
                c += abs(dp[u]);
                dp[v] += dp[u];
            }
            cost += rt.weight[v] * ((c - abs(dp[v])) / 2);
        }
        cout << cost << "\n";

        for (int a : all) {
            virtual_tree[a].clear();
            dp[a] = 0;
        }

        for (int x : undo) {
            seen[x] = false;
            crystals[x] = 0;
        }
    }
}
