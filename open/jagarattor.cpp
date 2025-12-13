#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> BCT, bccs, cutpoint_children;
    vector<int> node_id, in, out, parent, edge_component;
    vector<bool> cutpoint;

    BlockCutTree(int n, int m, vector<vector<pair<int, int>>> &adj_list) : n(n), node_id(n + 1, -1), edge_component(m), cutpoint(n + 1, false) {
        tarjan(adj_list);
        build();
    }

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
                            bccs.emplace_back(vector{v});

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

        cutpoint_children.assign(m, {});
        for (int v = bccs.size(); v < m; v++) {
            for (int u : BCT[v])
                if (parent[u] == v) cutpoint_children[v].emplace_back(u);
            sort(cutpoint_children[v].begin(), cutpoint_children[v].end(), [&](int x, int y) { return in[x] < in[y]; });
        }
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
            sccs++;
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
            } while (u != v);
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
    for (int i = 0; i < m; i++) {
        auto &[u, v, a, b] = pipes[i];
        cin >> u >> v >> a >> b;

        adj_list[u].emplace_back(v, i);
        adj_list[v].emplace_back(u, i);
    }
    for (int v = 1; v <= n; v++) sort(adj_list[v].begin(), adj_list[v].end(), [&](auto &p1, auto &p2) { return p1.first < p2.first; });

    auto encode = [&](int i, int dir) { return (i << 1) | dir; };
    auto decode = [&](int state) { return make_pair(state >> 1, state & 1); };

    vector<int> next(2 * m);
    for (int i = 0; i < m; i++) {
        auto [u, v, a, b] = pipes[i];
        int j = lower_bound(adj_list[v].begin(), adj_list[v].end(), a, [&](auto p, int x) { return p.first < x; })->second,
            k = lower_bound(adj_list[u].begin(), adj_list[u].end(), b, [&](auto p, int x) { return p.first < x; })->second;
        next[encode(i, 0)] = encode(j, pipes[j][0] != v);
        next[encode(i, 1)] = encode(k, pipes[k][0] != u);
    }

    BlockCutTree bct(n, m, adj_list);
    vector<array<int, 3>> states(2 * m);
    for (int i = 0; i < m; i++) {
        states[2 * i] = {pipes[i][1], bct.edge_component[i], encode(i, 0)};
        states[2 * i + 1] = {pipes[i][0], bct.edge_component[i], encode(i, 1)};
    }
    sort(states.begin(), states.end());

    int count = 0;
    vector<int> base_nodes(2 * m);
    vector<vector<int>> node_states;
    for (int i = 0; i < 2 * m;) {
        int v = states[i][0], c = states[i][1];
        node_states.emplace_back();
        for (; i < 2 * m && states[i][0] == v && states[i][1] == c; i++) {
            int s = states[i][2];
            base_nodes[s] = count;
            node_states.back().emplace_back(s);
        }
        count++;
    }

    vector<vector<int>> adj_list_rat(count);
    for (int state = 0; state < 2 * m; state++) {
        int u = base_nodes[state], v = base_nodes[next[state]];
        if (u != v) adj_list_rat[u].emplace_back(v);
    }

    auto dedupe = [&](auto &v) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    for (auto &neighbors : adj_list_rat) dedupe(neighbors);

    vector<pair<int, int>> SR(Q);
    vector<int> T(Q);
    for (int i = 0; i < Q; i++) cin >> SR[i].first >> SR[i].second >> T[i];

    if (adjacent_find(SR.begin(), SR.end(), not_equal_to<>()) == SR.end()) {
        auto [s0, r0] = SR[0];
        vector<bool> reachable(n + 1, false);
        reachable[s0] = true;
        queue<int> q;
        q.emplace(s0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, i] : adj_list[v])
                if (!reachable[u] && u != r0) {
                    reachable[u] = true;
                    q.emplace(u);
                }
        }

        vector<int> start_groups;
        for (auto [u, i] : adj_list[r0])
            if (reachable[u]) start_groups.emplace_back(base_nodes[encode(i, pipes[i][0] != u)]);
        sort(start_groups.begin(), start_groups.end());
        start_groups.erase(unique(start_groups.begin(), start_groups.end()), start_groups.end());

        vector<bool> visited(count, false), trapped(n + 1, false);
        for (int g : start_groups) {
            visited[g] = true;
            q.emplace(g);
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int s : node_states[v]) {
                auto [i, dir] = decode(next[s]);
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

    auto [sccs, component] = tarjan(count, adj_list_rat);
    vector<vector<int>> dag(sccs + 1);
    for (int v = 0; v < count; v++)
        for (int u : adj_list_rat[v])
            if (component[v] != component[u]) dag[component[v]].emplace_back(component[u]);
    for (int c = 1; c <= sccs; c++) dedupe(dag[c]);

    vector<int> Q_component(Q, -1);
    for (int q = 0; q < Q; q++) {
        auto [s, r] = SR[q];

        int vr = bct.node_id[r], vs = bct.node_id[s], c = -1;
        if (!bct.iscutpoint(vr)) c = bct.edge_component[adj_list[r][0].second];
        else {
            if (!bct.ancestor(vr, vs)) c = bct.parent[vr];
            else {
                auto &children = bct.cutpoint_children[vr];
                auto it = upper_bound(children.begin(), children.end(), bct.in[vs], [&](int count, int v) { return count < bct.in[v]; });
                if (it != children.begin()) c = *prev(it);
            }
        }
        if (!~c) continue;

        int i = lower_bound(states.begin(), states.end(), array<int, 3>{r, c, 0}) - states.begin();
        if (i < 2 * m && states[i][0] == r && states[i][1] == c) Q_component[q] = component[base_nodes[states[i][2]]];
    }

    vector<bool> visited(sccs + 1, false);
    vector<int> order;
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : dag[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    for (int c = 1; c <= sccs; c++)
        if (!visited[c]) dfs(dfs, c);

    auto deduped = T;
    dedupe(deduped);
    int s = deduped.size();
    vector<int> indices(n + 1, -1);
    for (int i = 0; i < s; i++) indices[deduped[i]] = i;

    vector<vector<int>> T_components(s);
    for (int state = 0; state < 2 * m; state++) {
        auto [i, dir] = decode(next[state]);
        if (~indices[pipes[i][dir ^ 1]]) T_components[indices[pipes[i][dir ^ 1]]].emplace_back(component[base_nodes[state]]);
    }

    const int size = 1 << 11;
    int k = (s + size - 1) / size;
    vector<vector<int>> blocks(k);
    vector<int> T_rev(Q);
    for (int qi = 0; qi < Q; qi++) {
        int t = indices[T[qi]];
        if (t != -1) {
            blocks[t / size].emplace_back(qi);
            T_rev[qi] = t;
        } else T_rev[qi] = -1;
    }

    vector<bool> trapped(Q, false);
    vector<bitset<size>> dp(sccs + 1);
    for (int b = 0; b < k; b++) {
        int tl = b * size, tr = min(s, tl + size);

        for (int c = 1; c <= sccs; c++) dp[c].reset();
        for (int t = tl; t < tr; t++)
            for (int c : T_components[t]) dp[c][t - tl] = true;

        for (int v : order)
            for (int u : dag[v]) dp[v] |= dp[u];

        for (int q : blocks[b]) {
            int c = Q_component[q];
            if (~c && tl <= T_rev[q] && T_rev[q] < tr && dp[c][T_rev[q] - tl]) trapped[q] = true;
        }
    }

    for (bool b : trapped) cout << b << "\n";
}
