#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    int t;
    vector<int> sets, seen;

    int find(int v) {
        if (seen[v] != t) {
            seen[v] = t;
            return sets[v] = v;
        }

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
        t++;
    }

    DisjointSets(int n) : t(1), sets(n), seen(n, 0) {
        iota(sets.begin(), sets.end(), 0);
    }
};

void build_virtual_tree(vector<vector<int>> &vt, vector<int> &vertices, const vector<int> &in, auto &&lca) {
    auto dedupe = [&](auto &v) {
        sort(v.begin(), v.end(), [&](int u, int v) { return in[u] < in[v]; });
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    dedupe(vertices);

    int m = vertices.size();
    for (int i = 0; i + 1 < m; i++) vertices.emplace_back(lca(vertices[i], vertices[i + 1]));
    dedupe(vertices);

    for (int i = 0; i + 1 < vertices.size(); i++) vt[lca(vertices[i], vertices[i + 1])].emplace_back(vertices[i + 1]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DisjointSets dsu(n + 1);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    vector<array<int, 3>> extra;
    vector<int> vertices;
    while (m--) {
        int a, b, w;
        cin >> a >> b >> w;

        if (dsu.unite(a, b)) {
            adj_list[a].emplace_back(b, w);
            adj_list[b].emplace_back(a, w);
        } else {
            extra.push_back({a, b, w});
            vertices.emplace_back(a);
            vertices.emplace_back(b);
        }
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(n + 1, 0), in(n + 1), inlabel(n + 1), ascendant(n + 1, 0), head(n + 2);
    vector<array<int, 30>> pref(n + 1);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();
        in[v] = count++;

        for (auto [u, w] : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                for (int b = 0; b < 30; b++) pref[u][b] = pref[v][b] + ((w >> b) & 1);
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);
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

    auto ancestor = [&](int u, int v) {
        return lca(u, v) == u;
    };

    auto path_OR = [&](int u, int v) {
        int OR = 0;
        for (int a = lca(u, v), b = 0; b < 30; b++)
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

    vector<int> indices(n + 1, -1);
    vector<vector<int>> vt(n + 1);
    vector<array<int, 3>> edges;
    build_virtual_tree(vt, vertices, in, lca);
    int V = vertices.size();
    for (int i = 0; i < V; i++) indices[vertices[i]] = i;
    for (int v : vertices)
        for (int u : vt[v]) edges.push_back({indices[v], indices[u], path_OR(v, u)});
    for (auto [u, v, w] : extra) edges.push_back({indices[u], indices[v], w});

    vector<array<int, 2>> queries(q);
    vector<int> OR(q), nodes, node_id(n + 1, -1);
    for (int i = 0; i < q; i++) {
        int s, t;
        cin >> s >> t;

        queries[i] = {s, t};
        OR[i] = path_OR(s, t);
        if (!~node_id[s]) {
            nodes.emplace_back(s);
            node_id[s] = nodes.size() - 1;
        }
        if (!~node_id[t]) {
            nodes.emplace_back(t);
            node_id[t] = nodes.size() - 1;
        }
    }

    vector<vector<array<int, 2>>> links(nodes.size());
    for (int k = 0; k < nodes.size(); k++) {
        int x = nodes[k];
        if (~indices[x]) {
            links[k].push_back({indices[x], 0});
            continue;
        }

        int j = -1;
        for (int i = 0; i < V; i++) {
            int v = vertices[i];
            if (ancestor(v, x) && (!~j || depth[v] > depth[vertices[j]])) j = i;
        }

        if (~j) {
            int v = vertices[j];
            for (int u : vt[v]) {
                int a = lca(x, u);
                if (a != v && ancestor(v, a) && ancestor(a, u)) {
                    links[k].push_back({j, path_OR(v, x)});
                    links[k].push_back({indices[u], path_OR(x, u)});
                    goto next;
                }
            }
            links[k].push_back({j, path_OR(v, x)});
        } else links[k].push_back({0, path_OR(vertices[0], x)});
        next:;
    }

    dsu.reset();
    unordered_map<int, vector<int>> memo;
    for (int i = 0; i < q; i++) {
        int b = 29;
        for (; ~b; b--)
            if ((OR[i] >> b) & 1) break;

        int m1 = (1 << (b + 1)) - 1;
        auto [s, t] = queries[i];
        for (; ~b; b--) {
            int m2 = m1 ^ (1 << b), m3 = ((1 << 30) - 1) ^ m2;
            if (!(OR[i] & m3)) {
                m1 = m2;
                continue;
            }

            auto [it, inserted] = memo.try_emplace(m2, V);
            auto &component = it->second;
            if (inserted) {
                dsu.reset();
                for (auto [u, v, w] : edges)
                    if (!(w & m3)) dsu.unite(u, v);
                for (int j = 0; j < V; j++) component[j] = dsu.find(j);
            }

            vector<int> l, r;
            for (auto [j, w] : links[node_id[s]])
                if (!(w & m3)) l.emplace_back(component[j]);
            for (auto [j, w] : links[node_id[t]])
                if (!(w & m3)) r.emplace_back(component[j]);

            for (int c1 : l)
                for (int c2 : r)
                    if (c1 == c2) {
                        m1 = m2;
                        goto done;
                    }
            done:;
        }
        cout << m1 << "\n";
    }
}
