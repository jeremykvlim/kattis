#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

vector<bool> tarjan(int n, int m, vector<vector<pair<int, int>>> &adj_list, vector<bool> &visited) {
    vector<int> order(n, 0), low(n, 0);
    vector<bool> bridge(m, false);
    int count = 0;

    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        for (auto [u, i] : adj_list[v])
            if (visited[u] && i != prev) {
                if (!order[u]) {
                    self(self, u, i);
                    low[v] = min(low[v], low[u]);
                    if (low[u] > order[v]) bridge[i] = true;
                } else low[v] = min(low[v], order[u]);
            }
    };
    for (int v = 0; v < n; v++)
        if (visited[v] && !order[v]) dfs(dfs, v);

    return bridge;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        auto &[x, y] = edges[i];
        cin >> x >> y;
        if (!~x) x = 0;
        if (!~y) y = 0;

        edges[i] = {x, y};
        adj_list[x].emplace_back(y, i);
        adj_list[y].emplace_back(x, i);
    }

    vector<bool> visited(n + 1, false);
    visited[0] = true;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, _] : adj_list[v])
            if (!visited[u]) {
                visited[u] = true;
                q.emplace(u);
            }
    }

    auto bridge = tarjan(n + 1, m, adj_list, visited);
    DisjointSets dsu(n + 1);
    for (int i = 0; i < m; i++) {
        auto [x, y] = edges[i];
        if (visited[x] && visited[y] && !bridge[i]) dsu.unite(x, y);
    }

    int count = 0;
    vector<int> root_component(n + 1, -1), component(n + 1, -1);
    for (int v = 0; v <= n; v++)
        if (visited[v]) {
            int v_set = dsu.find(v);
            if (!~root_component[v_set]) root_component[v_set] = count++;
            component[v] = root_component[v_set];
        }
    if (!count) {
        cout << "*0";
        exit(0);
    }

    vector<int> dp(count, 0);
    vector<vector<int>> adj_list_tree(count);
    for (int i = 0; i < m; i++) {
        auto [x, y] = edges[i];
        if (visited[x] && visited[y]) {
            int cx = component[x], cy = component[y];
            if (bridge[i]) {
                adj_list_tree[cx].emplace_back(cy);
                adj_list_tree[cy].emplace_back(cx);
            } else dp[cx] ^= 1;
        }
    }

    vector<int> prev(count, -1), order;
    prev[component[0]] = -2;
    auto dfs = [&](auto &&self, int v) -> void {
        order.emplace_back(v);
        for (int u : adj_list_tree[v])
            if (!~prev[u]) {
                prev[u] = v;
                self(self, u);
            }
    };
    dfs(dfs, component[0]);

    reverse(order.begin(), order.end());
    for (int v : order)
        for (int u : adj_list_tree[v])
            if (prev[u] == v) dp[v] ^= dp[u] + 1;
    cout << "*" << dp[component[0]];
}
