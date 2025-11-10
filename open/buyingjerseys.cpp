#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs;
    vector<int> node_id;
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
    }

    bool iscutpoint(int node) {
        return node >= bccs.size();
    }

    int size() {
        return BCT.size();
    }

    auto & operator[](int i) {
        return BCT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    string s;
    cin >> s;

    vector<int> delta(n + 1, 0);
    for (int i = 1; i <= n; i++) delta[i] = (s[i - 1] == '0') ? 1 : -1;

    BlockCutTree bct(n, adj_list);
    int bccs = bct.bccs.size(), blocks = bct.size();

    vector<int> weight(blocks, 0);
    for (int v = 1; v <= n; v++)
        if (bct.cutpoint[v]) weight[bct.node_id[v]] = delta[v];

    for (int c = 0; c < bccs; c++)
        for (int v : bct.bccs[c])
            if (!bct.cutpoint[v]) weight[c] += delta[v];

    vector<bool> visited(blocks, false);
    auto dfs1 = [&](auto &&self, auto &&other, int v, int prev = -1) -> array<int, 3> {
        visited[v] = true;
        int pos = weight[v], neg = -weight[v];
        for (int u : bct[v])
            if (u != prev) {
                auto dp = other(self, other, u, v);
                pos += max(dp[0], dp[2]);
                neg += max(dp[1], dp[2]);
            }
        return {pos, neg, max(abs(pos), abs(neg))};
    };

    auto dfs2 = [&](auto &&self, auto &&other, int v, int prev = -1) -> array<int, 3> {
        visited[v] = true;
        int pos = weight[v], neg = -weight[v], remove = 0;
        for (int u : bct[v])
            if (u != prev) {
                auto dp = self(self, other, u, v);
                pos += dp[0];
                neg += dp[1];
                remove += dp[2];
            }
        return {pos, neg, remove - 3};
    };

    int k = 0;
    for (int b = 0; b < blocks; b++)
        if (!bct.iscutpoint(b) && !visited[b]) k += dfs1(dfs1, dfs2, b)[2];

    for (int v = 1; v <= n; v++) k += !bct.cutpoint[v] && bct.node_id[v] == -1;
    cout << (n - k) / 2;
}
