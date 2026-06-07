#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> dist(n, -1);
    dist[0] = 0;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list[v])
            if (!~dist[u]) {
                dist[u] = dist[v] + 1;
                q.emplace(u);
            }
    }

    vector<int> order(n - 1);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int u, int v) { return dist[u] != dist[v] ? dist[u] < dist[v] : u < v; });

    vector<vector<int>> adj_list_tree(n);
    for (int v : order) {
        int p = n;
        for (int u : adj_list[v])
            if (dist[u] == dist[v] - 1) p = min(p, u);

        adj_list_tree[p].emplace_back(v);
        adj_list_tree[v].emplace_back(p);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(n, 0), prev(n, 0), inlabel(n), ascendant(n, 0), head(n + 1);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        tour.emplace_back(v, prev[v]);
        inlabel[v] = tour.size();

        for (int u : adj_list_tree[v])
            if (u != prev[v]) {
                prev[u] = v;
                depth[u] = depth[v] + 1;
                self(self, u);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);
    for (auto [v, p] : tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);

    auto lca = [&](int u, int v) -> int {
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
    };

    auto count = 0LL;
    for (int u = 0; int v : order) {
        count += depth[u] + depth[prev[v]] - 2 * depth[lca(u, prev[v])] + 1;
        u = v;
    }
    cout << count;
}
