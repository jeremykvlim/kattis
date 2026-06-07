#include <bits/stdc++.h>
using namespace std;

struct DominatorTree {
    int n;
    vector<int> DT, semidom, sets, label, order, depth, inlabel, ascendant, head, jump, len, start, pos, leaf, ladder;
    vector<vector<int>> adj_list_DT;
    vector<pair<int, int>> tour;

    DominatorTree(int n, const vector<vector<int>> &adj_list, int root = 1) : n(n), DT(n + 1, -1), semidom(n + 1, -1), sets(n + 1, 0),
                                                                              label(n + 1, 0), order(n + 1, -1), adj_list_DT(n + 1),
                                                                              depth(n + 1, 0), inlabel(n + 1), ascendant(n + 1), head(n + 2),
                                                                              jump(2 * n + 5), len(n + 1), start(n + 1), pos(n + 1), leaf(n + 1),
                                                                              ladder(9 * n) {
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

        auto lsb = [&](int x) {
            return x & -x;
        };

        int k = 1;
        vector<int> st(n + 1);
        auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
            st[depth[v]] = v;
            tour.emplace_back(v, prev);
            inlabel[v] = tour.size();
            pos[v] = k;
            jump[k++] = st[max(0, depth[v] - 3 * lsb(k))];
            leaf[v] = v;

            for (int u : adj_list_DT[v])
                if (u != prev) {
                    depth[u] = depth[v] + 1;
                    self(self, u, v);
                    head[inlabel[u]] = v;
                    if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
                    if (depth[leaf[v]] < depth[leaf[u]]) leaf[v] = leaf[u];
                    jump[k++] = st[max(0, depth[v] - 3 * lsb(k))];
                }
        };
        dfs(dfs, root, root);
        for (int i = 0; i < tour.size(); i++) {
            auto [v, p] = tour[i];
            if (!i || leaf[v] != leaf[p]) len[leaf[v]] = depth[leaf[v]] - depth[v] + 1;
        }

        for (int i = 0; auto [v, p] : tour) {
            ascendant[v] = ascendant[p] | lsb(inlabel[v]);
            st[depth[v]] = v;
            if (len[v]) {
                start[v] = i;
                for (int j = 0; j < min(max(3 * len[v], 6), depth[v] + 1); j++) ladder[i++] = st[depth[v] - j];
            }
        }
    }

    int find(int v, bool compress = false) {
        if (sets[v] == v) return compress ? -1 : v;

        int u = find(sets[v], true);
        if (!~u) return v;

        if (semidom[label[v]] > semidom[label[sets[v]]]) label[v] = label[sets[v]];
        sets[v] = u;
        return compress ? u : label[v];
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

    int kth_ancestor(int v, int k) {
        int d = depth[v] - k;
        if (int j = bit_floor(k / (4U))) v = jump[(pos[v] & -j) | j];
        return ladder[start[leaf[v]] + depth[ladder[start[leaf[v]]]] - d];
    }

    int & operator[](int i) {
        return DT[i];
    }
};

void build_virtual_tree(vector<vector<int>> &vt, vector<int> &vertices, vector<int> &prev, const vector<int> &in, auto &&lca) {
    auto dedupe = [&](auto &v) {
        sort(v.begin(), v.end(), [&](int u, int v) { return in[u] < in[v]; });
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    dedupe(vertices);

    int m = vertices.size();
    for (int i = 0; i + 1 < m; i++) vertices.emplace_back(lca(vertices[i], vertices[i + 1]));
    dedupe(vertices);

    for (int i = 0; i + 1 < vertices.size(); i++) vt[prev[vertices[i + 1]] = lca(vertices[i], vertices[i + 1])].emplace_back(vertices[i + 1]);
}

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
    vector<int> parent(n + 1, 0);
    auto dfs1 = [&](auto &&self, int v = 1) -> void {
        in[v] = count++;

        bool add = false;
        for (int u : adj_list[v])
            if (!~in[u]) {
                parent[u] = v;
                adj_list_tree[v].emplace_back(u);
                self(self, u);
            } else add = true;

        out[v] = count;
        if (add) extra.emplace_back(v);
    };
    dfs1(dfs1);

    vector<long long> visited(n + 1, 0), masks(n + 1, 0);
    queue<int> q;
    for (int i = 0; i < extra.size(); i++) {
        auto mask = 1LL << i;
        visited[extra[i]] |= mask;
        q.emplace(extra[i]);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (!(visited[u] & mask)) {
                    visited[u] |= mask;
                    q.emplace(u);
                }
        }
        masks[extra[i]] |= mask;
    }

    auto dfs2 = [&](auto &&self, int v = 1) -> void {
        for (int u : adj_list_tree[v]) {
            self(self, u);
            masks[v] |= masks[u];
        }
    };
    dfs2(dfs2);

    vector<bool> seen(n + 1, false);
    vector<int> labels(n + 1), prev(n + 1);
    vector<vector<int>> vt(n + 1);
    vector<vector<long long>> dp(n + 1, vector<long long>(5, 0));
    while (Q--) {
        int k, a;
        cin >> k >> a;

        labels.resize(a);
        for (int &i : labels) {
            cin >> i;

            seen[i] = true;
        }
        build_virtual_tree(vt, labels, prev, in, [&](int u, int v) { return dt.lca(u, v); });

        auto dfs3 = [&](auto &&self, int v) -> void {
            if (!seen[v]) {
                dp[v][0] = 1;
                for (int u : vt[v]) {
                    self(self, u);
                    vector<long long> temp(k + 1, 0);
                    for (int i = 0; i <= k; i++)
                        for (int j = 1; j <= k - i; j++) temp[i + j] += dp[v][i] * dp[u][j];
                    dp[v] = temp;
                }
            }

            vector<int> c;
            auto reach = 0LL;
            for (int u = v; prev[u] && c.size() + vt[prev[u]].size() - 1 <= k; u = prev[u])
                for (int t : vt[prev[u]])
                    if (t != u) {
                        c.emplace_back(t);
                        reach |= visited[t];
                    }

            auto ancestor = [&](int v, int u) {
                return in[v] <= in[u] && in[u] < out[v];
            };

            auto valid = [&](int a) {
                if (masks[a] & reach) return false;
                for (int b : c)
                    if (ancestor(a, b)) return false;
                return true;
            };
            if (!valid(v)) return;

            int u = v;
            for (int i = __lg(n); ~i; i--)
                if (dt.depth[u] - dt.depth[min(1, prev[v])] >= 1 << i) {
                    int a = dt.kth_ancestor(u, 1 << i);
                    if (valid(a)) u = a;
                }
            dp[v][1] += dt.depth[v] - dt.depth[u] + 1;
        };
        dfs3(dfs3, labels[0]);

        cout << dp[labels[0]][k] << "\n";
        for (int i : labels) {
            prev[i] = 0;
            vt[i].clear();
            fill(dp[i].begin(), dp[i].end(), 0);
            seen[i] = false;
        }
    }
}