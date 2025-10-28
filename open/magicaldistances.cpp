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

vector<pair<int, int>> virtual_tree(vector<int> &vertices, const vector<int> &in, auto &&lca) {
    auto dedupe = [&](auto &v) {
        sort(v.begin(), v.end(), [&](int u, int v) { return in[u] < in[v]; });
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    dedupe(vertices);

    int m = vertices.size();
    for (int i = 0; i + 1 < m; i++) vertices.emplace_back(lca(vertices[i], vertices[i + 1]));
    dedupe(vertices);

    vector<pair<int, int>> vt_edges(vertices.size() - 1);
    for (int i = 0; i + 1 < vertices.size(); i++) vt_edges[i] = {lca(vertices[i], vertices[i + 1]), vertices[i + 1]};
    return vt_edges;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DisjointSets dsu1(n + 1);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    vector<array<int, 3>> extra;
    while (m--) {
        int a, b, w;
        cin >> a >> b >> w;

        if (dsu1.unite(a, b)) {
            adj_list[a].emplace_back(b, w);
            adj_list[b].emplace_back(a, w);
        } else extra.push_back({a, b, w});
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(n + 1), depth(n + 1, 0), in(n + 1), anc_mask(n + 1, 0), head(n + 2);
    vector<array<int, 31>> pref(n + 1);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
        tour.emplace_back(v, prev);
        index[v] = tour.size();
        in[v] = count++;

        for (auto [u, w] : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                for (int b = 0; b <= 30; b++) pref[u][b] = pref[v][b] + ((w >> b) & 1);
                self(self, u, v);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
    };
    dfs(dfs);
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

    auto path_OR = [&](int u, int v) {
        int OR = 0;
        for (int a = lca(u, v), b = 0; b <= 30; b++)
            if (pref[u][b] + pref[v][b] > 2 * pref[a][b]) OR |= 1 << b;
        return OR;
    };

    int q;
    cin >> q;

    if (extra.empty()) {
        while (q--) {
            int s, t;
            cin >> s >> t;
            cout << path_OR(s, t) << "\n";
        }
        exit(0);
    }

    int e = extra.size();
    vector<int> base(2 * e);
    for (int i = 0; i < e; i++) {
        base[2 * i] = extra[i][0];
        base[2 * i + 1] = extra[i][1];
    }
    sort(base.begin(), base.end(), [&](int u, int v) { return in[u] < in[v]; });
    base.erase(unique(base.begin(), base.end()), base.end());

    vector<int> indices(n + 1, -1);
    while (q--) {
        int s, t;
        cin >> s >> t;

        auto cities = base;
        cities.emplace_back(s);
        cities.emplace_back(t);
        auto vt_edges = virtual_tree(cities, in, lca);

        for (int i = 0; i < cities.size(); i++) indices[cities[i]] = i;
        int all = 0, k = vt_edges.size();
        vector<array<int, 3>> edges(k + e);
        for (int i = 0; i < k; i++) {
            auto [a, b] = vt_edges[i];
            int w = path_OR(a, b);
            edges[i] = {indices[a], indices[b], w};
            all |= w;
        }

        for (int i = 0; i < e; i++) {
            auto [a, b, w] = extra[i];
            edges[i + k] = {indices[a], indices[b], w};
            all |= w;
        }

        int remove = 0;
        DisjointSets dsu2(cities.size());
        for (int b = 30; ~b; b--)
            if ((all >> b) & 1) {
                int temp = remove | (1 << b);
                dsu2.reset();
                for (auto [u, v, w] : edges)
                    if (!(w & temp)) {
                        dsu2.unite(u, v);
                        if (dsu2.find(indices[s]) == dsu2.find(indices[t])) {
                            remove = temp;
                            goto next;
                        }
                    }
                next:;
            }
        cout << (all & ~remove) << "\n";
    }
}
