#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs;
    vector<int> node_id, depth, parent, inlabel, ascendant, head;
    vector<bool> cutpoint;
    vector<pair<int, int>> tour;

    BlockCutTree(int n, vector<vector<int>> &adj_list) : n(n), node_id(n + 1, -1), cutpoint(n + 1, false) {
        tarjan(adj_list);
        build();
    };

    void tarjan(vector<vector<int>> &adj_list) {
        vector<int> order(n + 1, 0), low(n + 1, 0);
        stack<int> st;
        int count = 0;

        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            order[v] = low[v] = ++count;
            st.emplace(v);
            for (int u : adj_list[v])
                if (u != prev) {
                    if (!order[u]) {
                        self(self, u, v);
                        low[v] = min(low[v], low[u]);

                        if (low[u] >= order[v]) {
                            cutpoint[v] = (order[v] > 1 || order[u] > 2);
                            bccs.emplace_back(vector{v});

                            while (bccs.back().back() != u) {
                                bccs.back().emplace_back(st.top());
                                st.pop();
                            }
                        }
                    } else low[v] = min(low[v], order[u]);
                }
        };
        for (int v = 1; v <= n; v++)
            if (!order[v]) dfs(dfs, v);
    }

    void build() {
        int node = bccs.size();
        for (int v = 1; v <= n; v++)
            if (cutpoint[v]) node_id[v] = node++;

        int m = node;
        BCT.resize(m);
        inlabel.resize(m);
        depth.resize(m);
        parent.resize(m);
        ascendant.resize(m);
        head.resize(m + 1);
        node = 0;
        for (auto &comp : bccs) {
            for (int v : comp)
                if (!cutpoint[v]) node_id[v] = node;
                else {
                    BCT[node].emplace_back(node_id[v]);
                    BCT[node_id[v]].emplace_back(node);
                }
            node++;
        }

        auto lsb = [&](int x) {
            return x & -x;
        };

        auto dfs = [&](auto &&self, int v = 0) -> void {
            tour.emplace_back(v, parent[v]);
            inlabel[v] = tour.size();

            for (int u : BCT[v])
                if (u != parent[v]) {
                    parent[u] = v;
                    depth[u] = depth[v] + 1;
                    self(self, u);
                    head[inlabel[u]] = v;
                    if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
                }
        };
        dfs(dfs);
        for (auto [v, p] : tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);
    }

    int lca(int u, int v) {
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
    }

    bool on_path(int v, int u, int t) {
        int vu = lca(v, u), vt = lca(v, t), ut = lca(u, t);
        return vu == t || (vu == ut && vt == t) || (vu == vt && ut == t);
    }

    bool iscutpoint(int node) {
        return node >= bccs.size();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, d;
    cin >> n >> m >> d;

    vector<pair<int, int>> edges(m);
    vector<vector<int>> adj_list(n + 1);
    for (auto &[u, v] : edges) {
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    BlockCutTree bct(n, adj_list);
    while (d--) {
        int p, q, r, s;
        cin >> p >> q >> r >> s;

        p = bct.node_id[p], q = bct.node_id[q], r = bct.node_id[r], s = bct.node_id[s];
        int pq = bct.lca(p, q), rs = bct.lca(r, s);

        auto path = [&]() {
            if (bct.depth[pq] == bct.depth[rs]) return pq != rs;
            else if (bct.depth[pq] > bct.depth[rs]) {
                int t = bct.parent[pq];
                return !bct.on_path(r, s, pq) && (!bct.on_path(r, s, t) || bct.iscutpoint(pq));
            } else {
                int t = bct.parent[rs];
                return !bct.on_path(p, q, rs) && (!bct.on_path(p, q, t) || !bct.iscutpoint(rs));
            }
        };
        cout << (path() ? "YES\n" : "NO\n");
    }
}
