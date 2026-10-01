#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs;
    vector<int> node_id, depth, parent, inlabel, ascendant, head, child, offset, pos, base;
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
                            bccs.emplace_back(vector{v});

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
        inlabel.resize(m);
        depth.resize(m);
        parent.resize(m);
        ascendant.resize(m);
        head.resize(m + 1);
        child.resize(m, -1);
        offset.resize(m + 1);
        pos.resize(m);
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

        auto lsb = [&](int x) {
            return x & -x;
        };

        auto dfs = [&](auto &&self, int v = 0) -> void {
            tour.emplace_back(v, parent[v]);
            inlabel[v] = tour.size();

            for (int u : BCT[v])
                if (u != parent[v]) {
                    parent[u] = v;
                    depth[u] = depth[v] + 1;
                    self(self, u);
                    head[inlabel[u]] = v;
                    if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
                }
        };
        dfs(dfs);
        for (auto [v, p] : tour) {
            ascendant[v] = ascendant[p] | lsb(inlabel[v]);
            if (v && inlabel[v] == inlabel[p]) child[p] = v;
        }

        for (auto [v, p] : tour)
            if (!v || inlabel[v] != inlabel[p]) {
                offset[inlabel[v]] = base.size();

                for (int u = v; ~u; u = child[u]) {
                    base.emplace_back(u);
                    pos[u] = base.size() - 1;
                }
            }
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

    bool iscutpoint(int node) {
        return node >= bccs.size();
    }

    int climb(int v, int d) {
        while (d) {
            if (d <= pos[v] - offset[inlabel[v]]) return base[pos[v] - d];
            d -= pos[v] - offset[inlabel[v]] + 1;
            v = head[inlabel[v]];
        }
        return v;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<array<int, 3>> edges(m);
    vector<vector<int>> adj_list(n + 1);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    BlockCutTree bct(n, adj_list);
    int bccs = bct.bccs.size(), nodes = bct.BCT.size();
    for (auto &bcc : bct.bccs) sort(bcc.begin(), bcc.end());
    vector<int> cut(nodes, -1);
    for (int v = 1; v <= n; v++)
        if (bct.cutpoint[v]) cut[bct.node_id[v]] = v;

    vector<vector<vector<pair<int, long long>>>> adj_lists(bccs);
    for (int b = 0; b < bccs; b++) adj_lists[b].resize(bct.bccs[b].size());
    for (auto [u, v, w] : edges) {
        int b;
        if (!bct.cutpoint[u]) b = bct.node_id[u];
        else if (!bct.cutpoint[v]) b = bct.node_id[v];
        else {
            int x = bct.node_id[u], y = bct.node_id[v];
            b = bct.parent[bct.depth[x] > bct.depth[y] ? x : y];
        }

        int x = lower_bound(bct.bccs[b].begin(), bct.bccs[b].end(), u) - bct.bccs[b].begin(), y = lower_bound(bct.bccs[b].begin(), bct.bccs[b].end(), v) - bct.bccs[b].begin();
        adj_lists[b][x].emplace_back(y, w);
        adj_lists[b][y].emplace_back(x, w);
    }

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    auto dijkstra = [&](int b, int s) {
        vector<long long> dist(bct.bccs[b].size(), 1e18);
        dist[s] = 0;
        pq.emplace(0, s);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_lists[b][v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(dist[u], u);
                }
        }
        return dist;
    };

    vector<long long> dist(n + 1, 0);
    for (auto [b, p] : bct.tour)
        if (!bct.iscutpoint(b)) {
            int s = !b ? bct.bccs[b][0] : cut[p];
            auto d = dijkstra(b, lower_bound(bct.bccs[b].begin(), bct.bccs[b].end(), s) - bct.bccs[b].begin());
            for (int i = 0; i < bct.bccs[b].size(); i++) dist[bct.bccs[b][i]] = dist[s] + d[i];
        }

    vector<long long> times(q);
    vector<vector<array<int, 3>>> sweep(bccs);
    for (int i = 0; i < q; i++) {
        int s, t;
        cin >> s >> t;

        int u = bct.node_id[s], v = bct.node_id[t], a = bct.lca(u, v);
        if (bct.iscutpoint(a)) times[i] = dist[s] + dist[t] - 2 * dist[cut[a]];
        else {
            int x = u == a ? s : cut[bct.climb(u, bct.depth[u] - bct.depth[a] - 1)], y = v == a ? t : cut[bct.climb(v, bct.depth[v] - bct.depth[a] - 1)];
            times[i] = dist[s] - dist[x] + dist[t] - dist[y];
            sweep[a].push_back({(int) (lower_bound(bct.bccs[a].begin(), bct.bccs[a].end(), x) - bct.bccs[a].begin()), (int) (lower_bound(bct.bccs[a].begin(), bct.bccs[a].end(), y) - bct.bccs[a].begin()), i});
        }
    }

    for (int b = 0; b < bccs; b++) {
        sort(sweep[b].begin(), sweep[b].end());
        for (int l = 0, r = 1; l < sweep[b].size(); l = r++) {
            for (; r < sweep[b].size() && sweep[b][l][0] == sweep[b][r][0]; r++);
            auto d = dijkstra(b, sweep[b][l][0]);
            for (int i = l; i < r; i++) times[sweep[b][i][2]] += d[sweep[b][i][1]];
        }
    }

    for (auto time : times) cout << time << "\n";
}
