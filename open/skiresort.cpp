#include <bits/stdc++.h>
using namespace std;

struct DominatorTree {
    int n;
    vector<int> DT, semidom, sets, label, order, depth, in, out;
    vector<vector<int>> adj_list_DT, lift;
    DominatorTree(int n, const vector<vector<int>> &adj_list, int root = 1) : n(n), DT(n + 1, -1), semidom(n + 1, -1), sets(n + 1, 0), 
                                                                              label(n + 1, 0), order(n + 1, -1), adj_list_DT(n + 1),
                                                                              depth(n + 1), in(n + 1, 0), out(n + 1, 0), lift(__lg(n) + 1, vector<int>(n + 1, root)) {
        build(adj_list, root);
    }

    void build(const vector<vector<int>> &adj_list, int root) {
        vector<int> node(n + 1, -1), prev(n + 1, -1);
        vector<vector<int>> adj_list_time(n + 1);
        int count1 = 0;
        auto dfs1 = [&](auto &&self, int v) -> void {
            order[v] = label[count1] = semidom[count1] = sets[count1] = count1;
            node[count1++] = v;

            for (int u : adj_list[v]) {
                if (!~order[u]) {
                    self(self, u);
                    prev[order[u]] = order[v];
                }

                adj_list_time[order[u]].emplace_back(order[v]);
            }
        };
        dfs1(dfs1, root);

        vector<vector<int>> times(n + 1);
        vector<int> idom(n + 1);
        iota(idom.begin(), idom.end(), 0);
        for (int t1 = count1 - 1; ~t1; t1--) {
            for (int t2 : adj_list_time[t1]) semidom[t1] = min(semidom[t1], semidom[find(t2)]);
            if (t1) times[semidom[t1]].emplace_back(t1);
            for (int t2 : times[t1]) {
                int v = find(t2);
                idom[t2] = (semidom[v] == semidom[t2] ? semidom[t2] : v);
            }
            if (t1 > 1) sets[t1] = prev[t1];
        }
        for (int t = 1; t < count1; t++)
            if (idom[t] != semidom[t]) idom[t] = idom[idom[t]];
        for (int t = 1; t < count1; t++) DT[node[t]] = node[idom[t]];
        DT[root] = root;

        for (int v = 1; v <= n; v++)
            if (v != root) adj_list_DT[DT[v]].emplace_back(v);

        depth[root] = 1;
        int count2 = 0;
        auto dfs2 = [&](auto &&self, int v, int prev = -1) -> void {
            in[v] = count2++;
            if (~prev) {
                depth[v] = depth[prev] + 1;
                lift[0][v] = prev;
                for (int i = 1; i <= __lg(n); i++) lift[i][v] = lift[i - 1][lift[i - 1][v]];
            }

            for (int u : adj_list_DT[v])
                if (u != prev) self(self, u, v);

            out[v] = count2;
        };
        dfs2(dfs2, root);
    }

    int find(int v, bool compress = false) {
        if (sets[v] == v) return compress ? -1 : v;

        int u = find(sets[v], true);
        if (!~u) return v;

        if (semidom[label[v]] > semidom[label[sets[v]]]) label[v] = label[sets[v]];
        sets[v] = u;
        return compress ? u : label[v];
    }

    bool ancestor(int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    }

    int lca(int u, int v) {
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;

        if (depth[u] < depth[v]) swap(u, v);

        for (int i = __lg(n); ~i; i--)
            if (!ancestor(lift[i][u], v)) u = lift[i][u];

        return DT[u];
    }

    int level_ancestor(int v, int l) {
        if (!l) return v;

        for (int i = 0; i <= __lg(n); i++)
            if ((l >> i) & 1) v = lift[i][v];
        return v;
    }

    int & operator[](int i) {
        return DT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, Q;
    cin >> n >> m >> Q;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
    }
    DominatorTree dt(n, adj_list);

    int count = 0;
    vector<int> in(n + 1, -1), out(n + 1, -1), extra;
    vector<vector<int>> adj_list_tree(n + 1);
    auto dfs1 = [&](auto &&self, int v = 1) -> void {
        in[v] = count++;

        bool add = false;
        for (int u : adj_list[v])
            if (!~in[u]) {
                self(self, u);
                adj_list_tree[v].emplace_back(u);
            } else add = true;

        out[v] = count;
        if (add) extra.emplace_back(v);
    };
    dfs1(dfs1);

    vector<vector<bool>> visited(extra.size(), vector<bool>(n + 1, false));
    queue<int> q;
    for (int i = 0; i < extra.size(); i++) {
        visited[i][extra[i]] = true;
        q.emplace(extra[i]);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (!visited[i][u]) {
                    visited[i][u] = true;
                    q.emplace(u);
                }
        }
    }

    vector<long long> masks(n + 1, 0);
    for (int i = 0; i < extra.size(); i++) masks[extra[i]] |= 1LL << i;
    auto dfs2 = [&](auto &&self, int v = 1) -> void {
        for (int u : adj_list_tree[v]) {
            self(self, u);
            masks[v] |= masks[u];
        }
    };
    dfs2(dfs2);

    vector<bool> seen(n + 1, false);
    vector<int> labels(n + 1), vt(2 * n - 1), prev(n + 1);
    vector<vector<int>> next(n + 1);
    vector<vector<long long>> dp(n + 1, vector<long long>(5, 0));
    while (Q--) {
        int k, a;
        cin >> k >> a;

        for (int i = 0; i < a; i++) {
            cin >> labels[i];

            seen[labels[i]] = true;
        }
        sort(labels.begin(), labels.begin() + a, [&](int u, int v) { return in[u] < in[v]; });

        for (int i = 0; i < a; i++) vt[i] = labels[i];
        for (int i = 0; i < a - 1; i++) vt[i + a] = dt.lca(labels[i], labels[i + 1]);
        sort(vt.begin(), vt.begin() + 2 * a - 1, [&](int u, int v) { return in[u] < in[v]; });
        int size = unique(vt.begin(), vt.begin() + 2 * a - 1) - vt.begin();
        for (int i = 1; i < size; i++) next[prev[vt[i]] = dt.lca(vt[i - 1], vt[i])].emplace_back(vt[i]);

        auto dfs = [&](auto &&self, int v) -> void {
            if (!seen[v]) {
                dp[v][0] = 1;
                for (int u : next[v]) {
                    self(self, u);
                    vector<long long> temp(k + 1, 0);
                    for (int i = 0; i <= k; i++)
                        for (int j = 1; j <= k - i; j++) temp[i + j] += dp[v][i] * dp[u][j];
                    dp[v] = temp;
                }
            }

            vector<int> c;
            for (int u = v; prev[u] && c.size() + next[prev[u]].size() - 1 <= k; u = prev[u])
                for (int t : next[prev[u]])
                    if (t != u) c.emplace_back(t);

            auto ancestor = [&](int v, int u) {
                return in[v] <= in[u] && in[u] < out[v];
            };

            auto reachable = [&](int v, int u) {
                auto mask = masks[v];
                while (mask) {
                    int i = countr_zero((unsigned long long) mask);
                    if (visited[i][u]) return true;
                    mask &= mask - 1;
                }
                return false;
            };

            auto valid = [&](int a) {
                for (int b : c)
                    if (ancestor(a, b) || reachable(a, b)) return false;
                return true;
            };

            int l = -1, r = dt.depth[v] - dt.depth[prev[v]], m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (valid(dt.level_ancestor(v, m))) l = m;
                else r = m;
            }
            dp[v][1] += l + 1;
        };
        dfs(dfs, vt[0]);

        cout << dp[vt[0]][k] << "\n";
        for (int i = 0; i < size; i++) {
            prev[vt[i]] = 0;
            next[vt[i]].clear();
            fill(dp[vt[i]].begin(), dp[vt[i]].end(), 0);
        }
        for (int i = 0; i < a; i++) seen[labels[i]] = false;
    }
}
