#include <bits/stdc++.h>
using namespace std;

struct BlockCutTree {
    int n;
    vector<vector<int>> bccs;
    vector<int> edge_component, bcc_edges;
    vector<bool> cutpoint;

    BlockCutTree(int n, int m, vector<vector<pair<int, int>>> &adj_list) : n(n), edge_component(m, -1), cutpoint(n, false) {
        tarjan(adj_list);
    }

    void tarjan(vector<vector<pair<int, int>>> &adj_list) {
        vector<int> order(n, 0), low(n, 0);
        stack<int> st_v, st_e;
        int count = 0;

        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            order[v] = low[v] = ++count;
            st_v.emplace(v);
            int children = 0;
            for (auto [u, i] : adj_list[v])
                if (u != prev) {
                    if (!order[u]) {
                        children++;
                        st_e.emplace(i);
                        self(self, u, v);
                        low[v] = min(low[v], low[u]);

                        if (low[u] >= order[v]) {
                            int j, edges = 0;
                            do {
                                j = st_e.top();
                                st_e.pop();
                                edge_component[j] = bccs.size();
                                edges++;
                            } while (j != i);

                            if (~prev) cutpoint[v] = true;
                            bccs.emplace_back(vector<int>{v});
                            bcc_edges.emplace_back(edges);

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

            if (!~prev && children > 1) cutpoint[v] = true;
        };
        for (int v = 0; v < n; v++)
            if (!order[v]) dfs(dfs, v);
    }

    bool bridge(int i) {
        return bcc_edges[edge_component[i]] == 1;
    }
};

pair<vector<int>, vector<vector<int>>> two_edge_components(int n, vector<vector<pair<int, int>>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), tec(n, -1);
    vector<vector<int>> tecs;
    stack<int> st;
    int count = 0;

    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);

        for (auto [u, i] : adj_list[v])
            if (i != prev) {
                if (!order[u]) {
                    self(self, u, i);
                    low[v] = min(low[v], low[u]);
                } else low[v] = min(low[v], order[u]);
            }

        if (order[v] == low[v]) {
            tecs.emplace_back();
            int u;
            do {
                u = st.top();
                st.pop();
                tec[u] = tecs.size() - 1;
                tecs.back().emplace_back(u);
            } while (u != v);
        }
    };

    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return {tec, tecs};
}

vector<int> z_function(const vector<int> &s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> b(n), e(n);
    for (int i = 0; i < n; i++) {
        cin >> b[i] >> e[i];
        b[i]--;
        e[i]--;
    }

    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 0; i < m; i++) {
        auto &[x, y] = edges[i];
        cin >> x >> y;

        adj_list[x - 1].emplace_back(y - 1, i);
        adj_list[y - 1].emplace_back(x - 1, i);
    }

    BlockCutTree bct(n, m, adj_list);
    auto [tec, tecs] = two_edge_components(n, adj_list);

    int ecs = tecs.size();
    vector<int> tec_edges(ecs, 0);
    for (int i = 0; i < m; i++)
        if (!bct.bridge(i)) {
            auto [u, v] = edges[i];
            tec_edges[tec[u]]++;
        }

    vector<int> bcc_blocks(ecs, 0);
    vector<bool> simple_cycles(ecs, true), even_cycle(ecs, false);
    for (int bcc = 0; bcc < bct.bccs.size(); bcc++) {
        if (bct.bcc_edges[bcc] <= 1) continue;
        int c = tec[bct.bccs[bcc][0]];
        bcc_blocks[c]++;
        if (bct.bcc_edges[bcc] > bct.bccs[bcc].size()) simple_cycles[c] = false;
        else if (bct.bcc_edges[bcc] == bct.bccs[bcc].size() && !(bct.bccs[bcc].size() & 1)) even_cycle[c] = true;
    }

    vector<int> type(ecs, -1);
    for (int c = 0; c < ecs; c++) {
        if (!tec_edges[c]) continue;
        if (!simple_cycles[c]) type[c] = 1;
        else {
            if (bcc_blocks[c] == 1) type[c] = 0;
            else {
                if (even_cycle[c]) type[c] = 1;
                else type[c] = 2;
            }
        }
    }

    vector<int> seen(1e6, -1), pos(n), delta(1e6, 0), count(1e6, 0), indices(1e6);
    for (int c = 0; c < ecs; c++) {
        if (type[c] == -1) {
            for (int i : tecs[c])
                if (b[i] != e[i]) {
                    cout << "impossible";
                    exit(0);
                }
            continue;
        }

        int k = tecs[c].size();
        if (!type[c]) {
            vector<int> order;
            int v = tecs[c][0], prev = -1;
            do {
                order.emplace_back(v);
                int t = -1;
                for (auto [u, id] : adj_list[v])
                    if (!bct.bridge(id) && u != prev) {
                        t = u;
                        break;
                    }
                prev = v;
                v = t;
            } while (v != tecs[c][0]);

            if (k != order.size()) {
                cout << "impossible";
                exit(0);
            }

            vector<int> s;
            for (int i : order) s.emplace_back(b[i]);
            s.emplace_back(-1);
            for (int i : order) s.emplace_back(e[i]);
            for (int i : order) s.emplace_back(e[i]);
            auto z = z_function(s);
            for (int i = 1; i < s.size(); i++)
                if (z[i] >= k) goto next;

            cout << "impossible";
            exit(0);
            next:;
            continue;
        }

        vector<int> animals;
        if (type[c] == 1) {
            for (int i : tecs[c]) {
                int begin = b[i], end = e[i];
                if (seen[begin] != c) {
                    seen[begin] = c;
                    delta[begin] = count[begin] = 0;
                    animals.emplace_back(begin);
                }
                if (seen[end] != c) {
                    seen[end] = c;
                    delta[end] = count[end] = 0;
                    animals.emplace_back(end);
                }
                delta[begin]++;
                delta[end]--;
            }

            for (int a : animals)
                if (delta[a]) {
                    cout << "impossible";
                    exit(0);
                }
            continue;
        }

        bool unique = true;
        for (int i : tecs[c]) {
            int begin = b[i], end = e[i];
            if (seen[begin] != c) {
                seen[begin] = c;
                delta[begin] = count[begin] = 0;
                animals.emplace_back(begin);
            }
            if (seen[end] != c) {
                seen[end] = c;
                delta[end] = count[end] = 0;
                animals.emplace_back(end);
            }
            delta[begin]++;
            delta[end]--;

            if (++count[begin] == 1) indices[begin] = i;
            else unique = false;
        }

        for (int a : animals)
            if (delta[a]) {
                cout << "impossible";
                exit(0);
            }
        if (!unique) continue;

        for (int i = 0; i < k; i++) pos[tecs[c][i]] = i;

        vector<int> p(k);
        for (int i = 0; i < k; i++) p[pos[indices[e[tecs[c][i]]]]] = i;

        int cycles = 0;
        vector<bool> visited(k, false);
        for (int i = 0; i < k; i++) {
            if (visited[i]) continue;
            for (int j = i; !visited[j]; j = p[j]) visited[j] = true;
            cycles++;
        }

        if ((k - cycles) & 1) {
            cout << "impossible";
            exit(0);
        }
    }
    cout << "possible";
}
