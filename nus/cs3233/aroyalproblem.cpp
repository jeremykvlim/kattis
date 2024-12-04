#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, components, lift;
    vector<int> node_id, depth, in, out;
    vector<bool> cutpoint;

    BlockCutTree(int n, vector<vector<int>> &adj_list) : n(n), node_id(n + 1, -1), cutpoint(n + 1, false) {
        tarjan(adj_list);
        build();
    };

    void tarjan(vector<vector<int>> &adj_list) {
        vector<int> order(n + 1), low(n + 1);
        stack<int> st;
        int count = 0;

        auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
            order[v] = low[v] = ++count;
            st.emplace(v);
            for (int u : adj_list[v])
                if (u != prev) {
                    if (!order[u]) {
                        self(self, u, v);
                        low[v] = min(low[v], low[u]);

                        if (low[u] >= order[v]) {
                            cutpoint[v] = (order[v] > 1 || order[u] > 2);
                            components.emplace_back(vector<int>{v});

                            while (components.back().back() != u) {
                                components.back().emplace_back(st.top());
                                st.pop();
                            }
                        }
                    } else low[v] = min(low[v], order[u]);
                }
        };
        dfs(dfs);
    }

    void build() {
        int node = components.size();
        for (int v = 1; v <= n; v++)
            if (cutpoint[v]) node_id[v] = node++;

        int m = node;
        BCT.resize(m);
        lift.assign(m, vector<int>(__lg(m)));
        depth.resize(m);
        in.resize(m);
        out.resize(m);
        node = 0;
        for (auto &comp : components) {
            for (int v : comp)
                if (!cutpoint[v]) node_id[v] = node;
                else {
                    BCT[node].emplace_back(node_id[v]);
                    BCT[node_id[v]].emplace_back(node);
                }
            node++;
        }

        int count = 0;
        auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
            in[v] = count++;
            if (~prev) {
                depth[v] = depth[prev] + 1;
                lift[v][0] = prev;
                for (int i = 1; i < __lg(m); i++) lift[v][i] = lift[lift[v][i - 1]][i - 1];
            }

            for (int u : BCT[v])
                if (u != prev) self(self, u, v);

            out[v] = count;
        };
        dfs(dfs);
    }

    int lca(int u, int v) {
        auto ancestor = [&](int v, int u) {
            return in[v] <= in[u] && in[u] < out[v];
        };
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;

        if (depth[u] < depth[v]) swap(u, v);
        for (int i = __lg(BCT.size()) - 1; ~i; i--)
            if (!ancestor(lift[u][i], v)) u = lift[u][i];

        return lift[u][0];
    }

    bool on_path(int v, int u, int t) {
        int vu = lca(v, u), vt = lca(v, t), ut = lca(u, t);
        return vu == t || (vu == ut && vt == t) || (vu == vt && ut == t);
    }

    bool iscutpoint(int node) {
        return node >= components.size();
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
                int t = bct.lift[pq][0];
                return !bct.on_path(r, s, pq) && (!bct.on_path(r, s, t) || bct.iscutpoint(pq));
            } else {
                int t = bct.lift[rs][0];
                return !bct.on_path(p, q, rs) && (!bct.on_path(p, q, t) || !bct.iscutpoint(rs));
            }
        };
        cout << (path() ? "YES\n" : "NO\n");
    }
}
