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

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    vector<vector<pair<int,long long>>> adj_list_regular(n), adj_list_transpose(n);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;

        adj_list_regular[u].emplace_back(v, w);
        adj_list_transpose[v].emplace_back(u, w);
    }

    int s, t;
    cin >> s >> t;

    vector<long long> dist(n);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    auto dijkstra = [&](int src, auto &adj_list) {
        fill(dist.begin(), dist.end(), 1e18);
        dist[src] = 0;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
        return dist;
    };
    auto dist_s = dijkstra(s, adj_list_regular), dist_t = dijkstra(t, adj_list_transpose);

    vector<bool> on_path(n, false);
    for (int v = 0; v < n; v++) on_path[v] = dist_s[v] != 1e18 && dist_t[v] != 1e18 && dist_s[v] + dist_t[v] == dist_s[t];

    int k = 1;
    vector<int> compress(n, 0), vertices{-1};
    for (int v = 0; v < n; v++)
        if (on_path[v]) {
            compress[v] = k++;
            vertices.emplace_back(v);
        }

    vector<vector<int>> dag(k);
    for (auto [u, v, w] : edges)
        if (on_path[u] && on_path[v] && dist_s[u] + w + dist_t[v] == dist_s[t]) dag[compress[u]].emplace_back(compress[v]);

    DominatorTree dt(k - 1, dag, compress[s]);
    vector<int> stations;
    for (int i = 1; i < k; i++)
        if (dt.ancestor(i, compress[t])) stations.emplace_back(vertices[i]);

    sort(stations.begin(), stations.end());
    for (int v : stations) cout << v << " ";
}
