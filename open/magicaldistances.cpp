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

    vector<pair<int, int>> bfs_edge(n + 1, {0, 0});
    vector<int> depth(n + 1, 0);
    vector<vector<int>> adj_list_bfs(n + 1);
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
    vector<vector<int>> lift(n + 1, vector<int>(lg)), OR_lift(n + 1, vector<int>(lg));
    for (int v = 1; v <= n; v++) tie(lift[v][0], OR_lift[v][0]) = bfs_edge[v];

    for (int i = 1; i < lg; i++)
        for (int v = 1; v <= n; v++) {
            lift[v][i] = lift[lift[v][i - 1]][i - 1];
            OR_lift[v][i] = OR_lift[v][i - 1] | OR_lift[lift[v][i - 1]][i - 1];
        }

    vector<unordered_map<int, pair<int, int>>> memo(n + 1);
    auto lca = [&](int u, int v) {
        if (memo[u].count(v)) return memo[u][v];
        if (memo[v].count(u)) return memo[v][u];
        if (depth[u] < depth[v]) swap(u, v);

        int diff = depth[u] - depth[v], OR_u = 0, OR_v = 0;
        for (int k = 0; k < lg; k++)
            if ((diff >> k) & 1) {
                OR_u |= OR_lift[u][k];
                u = lift[u][k];
            }
        if (u == v) return memo[u][v] = memo[v][u] = {u, OR_u};

        for (int i = lg - 1; ~i; i--)
            if (lift[u][i] != lift[v][i]) {
                OR_u |= OR_lift[u][i];
                OR_v |= OR_lift[v][i];
                u = lift[u][i];
                v = lift[v][i];
            }

        return memo[u][v] = memo[v][u] = {lift[u][0], OR_u | OR_v};
    };

    auto ancestor = [&](int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    };

    vector<int> base_V;
    vector<array<int, 3>> base_E;
    for (int v = 1; v <= n; v++)
        for (auto [u, w] : adj_list[v])
            if (v < u)
                if (bfs_edge[u] != make_pair(v, w) && bfs_edge[v] != make_pair(u, w)) {
                    base_V.emplace_back(v);
                    base_V.emplace_back(u);
                    base_E.push_back({v, u, w});
                }

    auto prune = [&](auto &v) {
        sort(v.begin(), v.end(), [&](int u, int v) {return in[u] < in[v];});
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    prune(base_V);
    int size = base_V.size();
    for (int i = 1; i < size; i++) base_V.emplace_back(lca(base_V[i - 1], base_V[i]).first);
    prune(base_V);

    int Q;
    cin >> Q;

    vector<int> indices(n + 1, -1);
    while (Q--) {
        int s, t;
        cin >> s >> t;
        if (s > t) swap(s, t);

        auto V = base_V;
        V.emplace_back(s);
        V.emplace_back(t);
        prune(V);
        if (V.size() != base_V.size()) {
            auto add = [&](int v) {
                int i = lower_bound(V.begin(), V.end(), v, [&](int u, int v) {return in[u] < in[v];}) - V.begin();
                if (i) V.emplace_back(lca(V[i - 1], v).first);
                if (i < V.size() - 1) V.emplace_back(lca(v, V[i + 1]).first);
            };

            add(s);
            add(t);
            prune(V);
        }

        for (int i = 0; i < V.size(); i++) indices[V[i]] = i;

        vector<array<int, 3>> E;
        for (auto [u, v, w] : base_E) E.push_back({indices[u], indices[v], w});

        stack<int> st;
        for (int v : V) {
            while (!st.empty() && !(ancestor(st.top(), v))) st.pop();
            if (!st.empty()) {
                int u = st.top();
                E.push_back({indices[u], indices[v], lca(u, v).second});
            }
            st.emplace(v);
        }

        vector<int> active(E.size());
        iota(active.begin(), active.end(), 0);
        int cost = 0;
        for (int b = __lg((*max_element(E.begin(), E.end(), [](auto e1, auto e2) {return e1[2] < e2[2];}))[2]); ~b; b--) {
            DisjointSets dsu(V.size());
            vector<int> temp;
            for (int i : active) {
                auto [u, v, w] = E[i];
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
