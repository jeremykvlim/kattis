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
            if (v != root) adj_list_DT[DT[v]].emplace_back(v);
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

    int n, m;
    cin >> n >> m;

    vector<long long> subtree_size(n + 1);
    for (int v = 1; v <= n; v++) cin >> subtree_size[v];

    vector<pair<int, int>> edges(m);
    vector<vector<int>> adj_list(n + 1);
    for (auto &[u, v] : edges) {
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> dist(n + 1, -1);
    dist[1] = 0;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list[v])
            if (!~dist[u]) {
                dist[u] = dist[v] + 1;
                q.emplace(u);
            }
    }

    vector<vector<int>> dag(n + 1);
    for (auto [u, v] : edges)
        if (dist[u] + 1 == dist[v]) dag[u].emplace_back(v);
        else if (dist[v] + 1 == dist[u]) dag[v].emplace_back(u);

    DominatorTree dt(n, dag);
    auto dfs = [&](auto &&self, int v = 1) -> void {
        for (int u : dt.adj_list_DT[v]) {
            self(self, u);
            subtree_size[v] += subtree_size[u];
        }
    };
    dfs(dfs);

    for (auto [u, v] : edges)
        if (dist[u] + 1 == dist[v] && dt[v] == u) cout << subtree_size[v] << "\n";
        else if (dist[v] + 1 == dist[u] && dt[u] == v) cout << subtree_size[u] << "\n";
        else cout << "0\n";
}
