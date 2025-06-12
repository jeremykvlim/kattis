#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs;
    vector<int> node_id, in, out, parent, edge_component;
    vector<bool> cutpoint;

    BlockCutTree(int n, int m, vector<vector<pair<int, int>>> &adj_list) : n(n), node_id(n + 1, -1), cutpoint(n + 1, false), edge_component(m) {
        tarjan(adj_list);
        build();
    };

    void tarjan(vector<vector<pair<int, int>>> &adj_list) {
        vector<int> order(n + 1, 0), low(n + 1, 0);
        stack<int> st_v, st_e;
        int count = 0;

        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            order[v] = low[v] = ++count;
            st_v.emplace(v);
            for (auto [u, i] : adj_list[v])
                if (u != prev) {
                    if (!order[u]) {
                        st_e.emplace(i);
                        self(self, u, v);
                        low[v] = min(low[v], low[u]);

                        if (low[u] >= order[v]) {
                            int j;
                            do {
                                j = st_e.top();
                                st_e.pop();

                                edge_component[j] = bccs.size();
                            } while (j != i);

                            cutpoint[v] = (order[v] > 1 || order[u] > 2);
                            bccs.emplace_back(vector<int>{v});

                            while (bccs.back().back() != u) {
                                bccs.back().emplace_back(st_v.top());
                                st_v.pop();
                            }
                        }
                    } else {
                        if (order[u] < order[v]) st_e.emplace(i);
                        low[v] = min(low[v], order[u]);
                    }
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
        parent.resize(m);
        in.resize(m);
        out.resize(m);
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

        int count = 0;
        auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
            in[v] = count++;
            if (~prev) parent[v] = prev;

            for (int u : BCT[v])
                if (u != prev) self(self, u, v);

            out[v] = count;
        };
        dfs(dfs);
    }

    bool ancestor(int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    }

    bool iscutpoint(int node) {
        return node >= bccs.size();
    }
};

pair<int, vector<int>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), component(n, 0);
    vector<bool> stacked(n, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
            } else if (stacked[u]) low[v] = min(low[v], order[u]);

        if (order[v] == low[v]) {
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
            } while (u != v);
            sccs++;
        }
    };

    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, Q;
    cin >> n >> m >> Q;

    vector<array<int, 4>> pipes(m);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    vector<unordered_map<int, int>> pipe_index(n + 1);
    for (int i = 0; i < m; i++) {
        auto &[u, v, a, b] = pipes[i];
        cin >> u >> v >> a >> b;

        adj_list[u].emplace_back(v, i);
        adj_list[v].emplace_back(u, i);
        pipe_index[u][v] = pipe_index[v][u] = i;
    }

    auto encode = [&](int i, int dir) { return (i << 1) | dir; };
    auto decode = [&](int state) -> pair<int, int> { return {state >> 1, state & 1}; };

    vector<int> next(2 * m);
    for (int i = 0; i < m; i++) {
        auto [u, v, a, b] = pipes[i];
        int j = pipe_index[v][a], k = pipe_index[u][b];
        next[encode(i, 0)] = encode(j, pipes[j][0] != v);
        next[encode(i, 1)] = encode(k, pipes[k][0] != u);
    }

    BlockCutTree bct(n, m, adj_list);
    vector<array<int, 3>> base_states(2 * m);
    for (int i = 0; i < m; i++) {
        base_states[2 * i] = {pipes[i][1], bct.edge_component[i], encode(i, 0)};
        base_states[2 * i + 1] = {pipes[i][0], bct.edge_component[i], encode(i, 1)};
    }
    sort(base_states.begin(), base_states.end());

    int states = 2 * m;
    vector<int> extra_states(states);
    for (int i = 0; i < 2 * m; states++) {
        auto [v, c, _] = base_states[i];
        for (; i < 2 * m && base_states[i][0] == v && base_states[i][1] == c; i++) extra_states[i] = states;
    }

    vector<vector<int>> adj_list_rat(states);
    for (int i = 0; i < 2 * m; i++) adj_list_rat[i].emplace_back(next[i]);
    for (int i = 0; i < 2 * m; i++) {
        int s1 = base_states[i][2], s2 = extra_states[i];
        adj_list_rat[s1].emplace_back(s2);
        adj_list_rat[s2].emplace_back(s1);
    }

    vector<pair<int, int>> SR(Q);
    vector<int> T(Q);
    for (int i = 0; i < Q; i++) cin >> SR[i].first >> SR[i].second >> T[i];

    if (adjacent_find(SR.begin(), SR.end(), not_equal_to<>()) == SR.end()) {
        auto [s, r] = SR[0];
        vector<bool> reachable(n + 1, false);
        reachable[s] = true;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, i] : adj_list[v])
                if (!reachable[u] && u != r) {
                    reachable[u] = true;
                    q.emplace(u);
                }
        }

        vector<int> valid;
        for (auto [u, i] : adj_list[r])
            if (reachable[u]) valid.emplace_back(encode(i, pipes[i][0] != u));

        vector<bool> visited(states, false), trapped(n + 1, false);
        for (int v : valid) {
            visited[v] = true;
            q.emplace(v);
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (v < 2 * m) {
                auto [i, dir] = decode(v);
                trapped[pipes[i][dir ^ 1]] = true;
            }

            for (int u : adj_list_rat[v])
                if (!visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
        }

        for (int t : T) cout << trapped[t] << "\n";
        exit(0);
    }

    auto [sccs, component] = tarjan(states, adj_list_rat);
    vector<vector<int>> dag(sccs);
    for (int v = 0; v < states; v++)
        for (int u : adj_list_rat[v])
            if (component[v] != component[u]) dag[component[v]].emplace_back(component[u]);

    vector<int> Q_component(Q, -1);
    for (int q = 0; q < Q; q++) {
        auto [s, r] = SR[q];

        int vr = bct.node_id[r], vs = bct.node_id[s], c = -1;
        if (!bct.iscutpoint(vr)) c = bct.edge_component[adj_list[r][0].second];
        else {
            for (auto [u, i] : adj_list[r]) {
                int comp = bct.edge_component[i];
                if ((bct.parent[comp] == vr && bct.ancestor(comp, vs)) || (bct.parent[vr] == comp && !bct.ancestor(vr, vs))){
                    c = comp;
                    break;
                }
            }
        }
        if (!~c) continue;

        array<int, 3> state{r, c, 0};
        int i = lower_bound(base_states.begin(), base_states.end(), state) - base_states.begin();
        if (i < 2 * m && base_states[i][0] == r && base_states[i][1] == c) Q_component[q] = component[extra_states[i]];
    }

    vector<bool> visited(sccs, false);
    vector<int> order;
    auto dfs2 = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : dag[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    for (int c = 0; c < sccs; c++)
        if (!visited[c]) dfs2(dfs2, c);

    auto deduped = T;
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());
    int s = deduped.size();
    vector<int> indices(n + 1, -1);
    for (int i = 0; i < s; i++) indices[deduped[i]] = i;

    vector<vector<int>> T_components(s);
    for (int v = 0; v < 2 * m; v++) {
        int u = next[v];
        auto [i, dir] = decode(u);
        if (~indices[pipes[i][dir ^ 1]]) T_components[indices[pipes[i][dir ^ 1]]].emplace_back(component[v]);
    }

    const int size = 1 << 11;
    int k = (s + size - 1) / size;
    vector<vector<int>> blocks(k);
    vector<int> T_rev(Q);
    for (int q = 0; q < Q; q++) {
        int t = indices[T[q]];
        blocks[t / size].emplace_back(q);
        T_rev[q] = t;
    }

    vector<bool> trapped(Q, false);
    vector<bitset<size>> dp(sccs);
    for (int b = 0; b < k; b++) {
        int tl = b * size, tr = min(s, tl + size);

        for (int c = 0; c < sccs; c++) dp[c].reset();
        for (int t = tl; t < tr; t++)
            for (int c : T_components[t]) dp[c][t - tl] = true;

        for (int v : order)
            for (int u : dag[v]) dp[v] |= dp[u];

        for (int q : blocks[b]) {
            int c = Q_component[q];
            if (~c && dp[c][T_rev[q] - tl]) trapped[q] = true;
        }
    }

    for (bool b : trapped) cout << b << "\n";
}
