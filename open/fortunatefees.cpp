#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs, lift;
    vector<int> node_id, depth, in, out, sizes, cuts;
    vector<long long> pref_sizes, pref_cuts;
    vector<bool> cutpoint;

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
                            bccs.emplace_back(vector<int>{v});

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
        lift.assign(__lg(m) + 1, vector<int>(m));
        depth.resize(m);
        in.resize(m);
        out.resize(m);
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

        sizes.resize(m);
        cuts.resize(m);
        for (int i = 0; i < bccs.size(); i++) sizes[i] = bccs[i].size();
        for (int i = bccs.size(); i < m; i++) cuts[i] = 1;

        pref_sizes.resize(m);
        pref_cuts.resize(m);
        pref_sizes[0] = sizes[0];
        pref_cuts[0] = cuts[0];

        int count = 0;
        auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
            in[v] = count++;
            if (~prev) {
                pref_sizes[v] = pref_sizes[prev] + sizes[v];
                pref_cuts[v] = pref_cuts[prev] + cuts[v];
                depth[v] = depth[prev] + 1;
                lift[0][v] = prev;
                for (int i = 1; i <= __lg(m); i++) lift[i][v] = lift[i - 1][lift[i - 1][v]];
            }

            for (int u : BCT[v])
                if (u != prev) self(self, u, v);

            out[v] = count;
        };
        dfs(dfs);
    }

    bool ancestor(int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    }

    int lca(int u, int v) {
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;

        if (depth[u] < depth[v]) swap(u, v);

        for (int i = __lg(BCT.size()); ~i; i--)
            if (!ancestor(lift[i][u], v)) u = lift[i][u];

        return lift[0][u];
    }

    bool iscutpoint(int node) {
        return node >= bccs.size();
    }

    long long query(int u, int v) {
        if (u == v) return 1;

        u = node_id[u], v = node_id[v];
        int uv = lca(u, v);
        return (pref_sizes[u] + pref_sizes[v] - 2LL * pref_sizes[uv] + sizes[uv]) -
               (pref_cuts[u] + pref_cuts[v] - 2LL * pref_cuts[uv] + cuts[uv]) +
               iscutpoint(u) + iscutpoint(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c, t;
    cin >> c >> t;

    vector<vector<int>> adj_list(c + 1);
    for (int i = 1; i <= c; i++) {
        int n;
        cin >> n;

        while (n--) {
            int s, e;
            char _;
            cin >> _ >> s >> _ >> e >> _;
            s++;
            e++;

            for (int j = s; j <= e; j++) {
                adj_list[i].emplace_back(j);
                adj_list[j].emplace_back(i);
            }
        }
    }

    BlockCutTree bct(c, adj_list);
    while (t--) {
        int u, v;
        cin >> u >> v;
        cout << bct.query(u + 1, v + 1) << "\n";
    }
}
