#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs;
    vector<int> node_id, index, depth, anc_mask, head, sizes, cuts;
    vector<long long> pref_sizes, pref_cuts;
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
        index.resize(m);
        depth.resize(m);
        anc_mask.resize(m);
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

        sizes.resize(m);
        cuts.resize(m);
        for (int i = 0; i < bccs.size(); i++) sizes[i] = bccs[i].size();
        for (int i = bccs.size(); i < m; i++) cuts[i] = 1;

        pref_sizes.resize(m);
        pref_cuts.resize(m);
        pref_sizes[0] = sizes[0];
        pref_cuts[0] = cuts[0];

        auto lsb = [&](int x) {
            return x & -x;
        };

        auto dfs = [&](auto &&self, int v = 0, int prev = 0) -> void {
            tour.emplace_back(v, prev);
            index[v] = tour.size();

            for (int u : BCT[v])
                if (u != prev) {
                    depth[u] = depth[v] + 1;
                    pref_sizes[u] = pref_sizes[v] + sizes[u];
                    pref_cuts[u] = pref_cuts[v] + cuts[u];
                    self(self, u, v);
                    head[index[u]] = v;
                    if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
                }
        };
        dfs(dfs);
        for (auto [v, p] : tour) anc_mask[v] = anc_mask[p] | lsb(index[v]);
    }

    int lca(int u, int v) {
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

            for (int j = s + 1; j <= e + 1; j++) {
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
