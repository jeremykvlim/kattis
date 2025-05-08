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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int a, b, w;
        cin >> a >> b >> w;

        adj_list[a].emplace_back(b, w);
        adj_list[b].emplace_back(a, w);
    }

    vector<vector<int>> adj_list_bfs(n + 1);
    vector<pair<int, int>> bfs_edge(n + 1, {0, 0});
    vector<int> depth(n + 1, 0);
    vector<bool> visited(n + 1, 0);
    visited[1] = 1;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, w] : adj_list[v])
            if (!visited[u]) {
                visited[u] = true;
                bfs_edge[u] = {v, w};
                depth[u] = depth[v] + 1;
                adj_list_bfs[v].emplace_back(u);
                adj_list_bfs[u].emplace_back(v);
                q.emplace(u);
            }
    }

    vector<int> in(n + 1, -1), out(n + 1, -1);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        in[v] = count++;
        for (int u : adj_list_bfs[v])
            if (u != prev) self(self, u, v);
        out[v] = count;
    };
    dfs(dfs);

    int lg = __lg(n) + 1;
    vector<vector<int>> lift(lg, vector<int>(n + 1, 0)), OR_lift(lg, vector<int>(n + 1, 0));
    for (int v = 1; v <= n; v++) tie(lift[0][v], OR_lift[0][v]) = bfs_edge[v];

    for (int i = 1; i < lg; i++)
        for (int v = 1; v <= n; v++) {
            lift[i][v] = lift[i - 1][lift[i - 1][v]];
            OR_lift[i][v] = OR_lift[i - 1][v] | OR_lift[i - 1][lift[i - 1][v]];
        }

    vector<unordered_map<int, pair<int, int>>> memo(n + 1);
    auto lca = [&](int u, int v) {
        if (memo[u].count(v)) return memo[u][v];
        if (memo[v].count(u)) return memo[v][u];
        if (depth[u] < depth[v]) swap(u, v);

        int diff = depth[u] - depth[v], OR_u = 0, OR_v = 0;
        for (int k = 0; k < lg; k++)
            if ((diff >> k) & 1) {
                OR_u |= OR_lift[k][u];
                u = lift[k][u];
            }
        if (u == v) return memo[u][v] = memo[v][u] = {u, OR_u};

        for (int i = lg - 1; ~i; i--)
            if (lift[i][u] != lift[i][v]) {
                OR_u |= OR_lift[i][u];
                OR_v |= OR_lift[i][v];
                u = lift[i][u];
                v = lift[i][v];
            }

        return memo[u][v] = memo[v][u] = {lift[0][u], OR_u | OR_v};
    };

    auto ancestor = [&](int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    };

    vector<int> base_v;
    vector<array<int, 3>> base_e;
    for (int v = 1; v <= n; v++)
        for (auto [u, w] : adj_list[v])
            if (v < u)
                if (bfs_edge[u] != make_pair(v, w) && bfs_edge[v] != make_pair(u, w)) {
                    base_v.emplace_back(v);
                    base_v.emplace_back(u);
                    base_e.push_back({v, u, w});
                }

    auto prune = [&](auto &v) {
        sort(v.begin(), v.end(), [&](int u, int v) { return in[u] < in[v]; });
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    prune(base_v);
    int size = base_v.size();
    for (int i = 1; i < size; i++) base_v.emplace_back(lca(base_v[i - 1], base_v[i]).first);
    prune(base_v);

    int Q;
    cin >> Q;

    vector<int> indices(n + 1, -1);
    while (Q--) {
        int s, t;
        cin >> s >> t;
        if (s > t) swap(s, t);

        auto vt = base_v;
        vt.emplace_back(s);
        vt.emplace_back(t);
        prune(vt);
        if (vt.size() != base_v.size()) {
            auto add = [&](int v) {
                int i = lower_bound(vt.begin(), vt.end(), v, [&](int u, int v) { return in[u] < in[v]; }) - vt.begin();
                if (i) vt.emplace_back(lca(vt[i - 1], v).first);
                if (i < vt.size() - 1) vt.emplace_back(lca(v, vt[i + 1]).first);
            };

            add(s);
            add(t);
            prune(vt);
        }

        for (int i = 0; i < vt.size(); i++) indices[vt[i]] = i;

        vector<array<int, 3>> edges;
        for (auto [u, v, w] : base_e) edges.push_back({indices[u], indices[v], w});

        stack<int> st;
        for (int v : vt) {
            while (!st.empty() && !(ancestor(st.top(), v))) st.pop();
            if (!st.empty()) {
                int u = st.top();
                edges.push_back({indices[u], indices[v], lca(u, v).second});
            }
            st.emplace(v);
        }

        vector<int> active(edges.size());
        iota(active.begin(), active.end(), 0);
        int cost = 0;
        for (int b = __lg((*max_element(edges.begin(), edges.end(), [](auto e1, auto e2) { return e1[2] < e2[2]; }))[2]); ~b; b--) {
            DisjointSets dsu(vt.size());
            vector<int> temp;
            for (int i : active) {
                auto [u, v, w] = edges[i];
                if (!((w >> b) & 1)) {
                    dsu.unite(u, v);
                    temp.emplace_back(i);
                }
            }

            if (dsu.find(indices[s]) == dsu.find(indices[t])) active = temp;
            else cost |= 1 << b;
        }
        cout << cost << "\n";
    }
}
