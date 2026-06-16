#include <bits/stdc++.h>
using namespace std;

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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<tuple<int, int, int>> edges(m);
    for (auto &[a, b, w] : edges) {
        cin >> a >> b >> w;
        a--;
        b--;
    }
    sort(edges.begin(), edges.end(), [&](auto e1, auto e2) { return get<2>(e1) < get<2>(e2); });

    ReachabilityTree rt(n, edges);
    int k = rt.n + 1;

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(k, 0), in(k), inlabel(k), ascendant(k, 0), head(k + 1);
    int count = 0;
    auto dfs1 = [&](auto &&self, int v, int prev) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();
        in[v] = count++;

        for (int u : rt.adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs1(dfs1, rt.n, rt.n);
    for (auto [v, p] : tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    int q;
    cin >> q;

    vector<bool> open(n, false);
    open[0] = true;
    set<pair<int, int>> stops{{in[0], 0}};
    while (q--) {
        int t, x;
        cin >> t >> x;
        x--;

        if (t == 1) {
            open[x] = true;
            stops.emplace(in[x], x);
        } else if (t == 2) {
            open[x] = false;
            stops.erase({in[x], x});
        } else {
            if (open[x]) cout << "0\n";
            else {
                int wet = INT_MAX;
                auto it = stops.lower_bound({in[x], -1});
                if (it != stops.begin()) wet = min(wet, rt.weight[lca(x, prev(it)->second)]);
                if (it != stops.end()) wet = min(wet, rt.weight[lca(x, it->second)]);
                cout << wet << "\n";
            }
        }
    }
}
