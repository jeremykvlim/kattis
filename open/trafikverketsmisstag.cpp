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

    void reset() {
        iota(sets.begin(), sets.end(), 0);
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(n), depth(n, 0), prev(n, 0), anc_mask(n, 0), head(n + 1);
    auto dfs1 = [&](auto &&self, int v = 0) -> void {
        tour.emplace_back(v, prev[v]);
        index[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                depth[u] = depth[v] + 1;
                self(self, u);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
    };
    dfs1(dfs1);
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

    vector<array<int, 3>> person(c);
    vector<bool> active(n, false);
    vector<int> indices(n, -1);
    for (int i = 0; i < c; i++) {
        int w, h;
        cin >> w >> h;
        w--;
        h--;

        person[i] = {w, h, lca(w, h)};
        active[w] = true;
        indices[w] = i;
    }

    DisjointSets dsu(n);
    auto temp = active;
    auto reset = [&]() -> void {
        dsu.reset();
        for (int i = 1; i < n; i++) {
            int v = tour[i].first;
            if (!active[v]) dsu.unite(prev[v], v);
        }
    };
    reset();

    auto deactivate = [&](int w) -> void {
        active[w] = false;
        if (w) dsu.unite(prev[w], w);
    };

    auto next = [&](int w, int h, int a) -> int {
        int w_set = dsu.find(w);
        if (active[w_set] && depth[w_set] >= depth[a]) return w_set;

        int v_set = dsu.find(h);
        if (active[v_set] && depth[v_set] >= depth[a]) return v_set;

        return -1;
    };

    vector<int> order;
    auto dfs2 = [&](auto &&self, int v) -> void {
        int i = indices[v];
        auto [w, h, a] = person[i];
        deactivate(w);
        for (;;) {
            int u = next(w, h, a);
            if (u == -1) break;
            self(self, u);
        }
        order.emplace_back(i);
    };
    for (int i = 0; i < n; i++)
        if (~indices[i] && active[i]) dfs2(dfs2, i);

    active = temp;
    reset();
    for (int i : order) {
        auto [w, h, a] = person[i];
        deactivate(w);
        if (next(w, h, a) != -1) {
            cout << "No";
            exit(0);
        }
    }

    cout << "Yes\n";
    for (int i : order) cout << i + 1 << " ";
}
