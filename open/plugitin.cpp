#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

struct DominatorTree {
    int n;
    vector<int> DT, semidom, sets, label, order;
    vector<vector<int>> adj_list_DT;
    DominatorTree(int n, const vector<vector<int>> &adj_list, int root = 1) : n(n), DT(n + 1, -1), semidom(n + 1, -1), sets(n + 1, 0),
                                                                              label(n + 1, 0), order(n + 1, -1), adj_list_DT(n + 1) {
        build(adj_list, root);
    }

    void build(const vector<vector<int>> &adj_list, int root) {
        vector<int> node(n + 1, -1), prev(n + 1, -1);
        vector<vector<int>> adj_list_time(n + 1);
        int count = 0;
        auto dfs = [&](auto &&self, int v) -> void {
            order[v] = label[count] = semidom[count] = sets[count] = count;
            node[count++] = v;

            for (int u : adj_list[v]) {
                if (!~order[u]) {
                    self(self, u);
                    prev[order[u]] = order[v];
                }

                adj_list_time[order[u]].emplace_back(order[v]);
            }
        };
        dfs(dfs, root);

        vector<vector<int>> times(n + 1);
        vector<int> idom(n + 1);
        iota(idom.begin(), idom.end(), 0);
        for (int t1 = count - 1; ~t1; t1--) {
            for (int t2 : adj_list_time[t1]) semidom[t1] = min(semidom[t1], semidom[find(t2)]);
            if (t1) times[semidom[t1]].emplace_back(t1);
            for (int t2 : times[t1]) {
                int v = find(t2);
                idom[t2] = (semidom[v] == semidom[t2] ? semidom[t2] : v);
            }
            if (t1 > 1) sets[t1] = prev[t1];
        }
        for (int t = 1; t < count; t++)
            if (idom[t] != semidom[t]) idom[t] = idom[idom[t]];
        for (int t = 1; t < count; t++) DT[node[t]] = node[idom[t]];
        DT[root] = root;

        for (int v = 1; v <= n; v++)
            if (v != root && ~DT[v]) adj_list_DT[DT[v]].emplace_back(v);
    }

    int find(int v, bool compress = false) {
        if (sets[v] == v) return compress ? -1 : v;

        int u = find(sets[v], true);
        if (!~u) return v;

        if (semidom[label[v]] > semidom[label[sets[v]]]) label[v] = label[sets[v]];
        sets[v] = u;
        return compress ? u : label[v];
    }

    int & operator[](int i) {
        return DT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, k;
    cin >> m >> n >> k;

    vector<pair<int, int>> edges(k);
    for (auto &[u, v] : edges) {
        cin >> u >> v;
        u--;
        v--;
    }
    auto [l, r, matches] = hopcroft_karp(m, n, edges);

    if (matches == n) {
        cout << matches;
        exit(0);
    }

    vector<int> unmatched(n, -1);
    int nodes = m + 1;
    for (int v = 0; v < n; v++)
        if (!~r[v]) unmatched[v] = nodes++;

    int root = nodes++;
    vector<vector<int>> adj_list(nodes);
    for (int v = 0; v < n; v++)
        if (!~r[v]) adj_list[root].emplace_back(unmatched[v]);

    for (auto [u, v] : edges)
        if (!~r[v]) adj_list[unmatched[v]].emplace_back(u + 1);
        else if (r[v] != u) adj_list[r[v] + 1].emplace_back(u + 1);

    DominatorTree dt(nodes - 1, adj_list, root);
    bool extra = false;
    for (int v = 1; v <= m; v++) {
        if (~dt[v]) {
            extra = true;
            if (dt[v] == root) {
                cout << matches + 2;
                exit(0);
            }
        }
    }
    cout << matches + extra;
}
