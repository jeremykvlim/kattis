#include <bits/stdc++.h>
using namespace std;

struct DominatorTree {
    int n;
    vector<int> DT, semidom, sets, label, order, index, depth, anc_mask, head;
    vector<vector<int>> adj_list_DT;
    vector<pair<int, int>> tour;

    DominatorTree(int n, const vector<vector<int>> &adj_list, int root = 1) : n(n), DT(n + 1, -1), semidom(n + 1, -1), sets(n + 1, 0),
                                                                              label(n + 1, 0), order(n + 1, -1), adj_list_DT(n + 1),
                                                                              index(n + 1), depth(n + 1, 0), anc_mask(n + 1), head(n + 2) {
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

        auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
            tour.emplace_back(v, prev);
            index[v] = tour.size();

            for (int u : adj_list_DT[v])
                if (u != prev) {
                    depth[u] = depth[v] + 1;
                    self(self, u, v);
                    head[index[u]] = v;
                    if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
                }
        };
        dfs(dfs, root);
        for (auto [v, p] : tour) anc_mask[v] = anc_mask[p] | lsb(index[v]);
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

    vector<int> heavy(n + 1, -1);
    auto hld = [&](auto &&self, int v = 1) -> int {
        int subtree_size = 1, largest = 0;
        for (int u : dt.adj_list_DT[v]) {
            int size = self(self, u);
            subtree_size += size;
            if (largest < size) {
                largest = size;
                heavy[v] = u;
            }
        }
        return subtree_size;
    };
    hld(hld);

    vector<int> chain_head(n + 1, 0), chain_pos(n + 1, 0), chain_vertex(n + 1, 0);
    int pos = 0;
    q.emplace(1);
    while (!q.empty()) {
        int h = q.front();
        q.pop();

        for (int v = h; v != -1; v = heavy[v]) {
            chain_vertex[pos] = v;
            chain_pos[v] = pos++;
            chain_head[v] = h;

            for (int u : dt.adj_list_DT[v])
                if (u != heavy[v]) q.push(u);
        }
    }

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
            for (int u = v; prev[u] && c.size() + vt[prev[u]].size() - 1 <= k; u = prev[u])
                for (int t : vt[prev[u]])
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

            int u = v;
            auto sum = 0LL;
            auto search = [&](int l, int r) {
                while (l + 1 < r) {
                    int mid = l + (r - l) / 2;

                    if (valid(chain_vertex[chain_pos[u] - mid])) l = mid;
                    else r = mid;
                }
                sum += l + 1;
            };

            auto climb = [&]() {
                int h = chain_head[u], len = chain_pos[u] - chain_pos[h] + 1;
                if (valid(h)) {
                    sum += len;
                    u = dt[h];
                    return true;
                } else {
                    search(-1, len);
                    dp[v][1] += sum;
                    return false;
                }
            };

            while (prev[v] && chain_head[u] != chain_head[prev[v]])
                if (!climb()) return;

            if (prev[v]) search(-1, chain_pos[u] - chain_pos[prev[v]] + 1);
            else {
                while (chain_head[u] != chain_head[1])
                    if (!climb()) return;
                search(-1, chain_pos[u] - chain_pos[1] + 1);
            }
            dp[v][1] += sum;
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
