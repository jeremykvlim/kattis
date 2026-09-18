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

struct TwoSATSystem {
    int n;
    vector<vector<int>> adj_list;

    TwoSATSystem(int n = 0) : n(n), adj_list(2 * n) {}

    int add_variable() {
        adj_list.emplace_back();
        adj_list.emplace_back();
        return n++;
    }

    void add_clause(int i, int j) {
        i = max(i << 1, -(i << 1 | 1));
        j = max(j << 1, -(j << 1 | 1));
        adj_list[i ^ 1].emplace_back(j);
        adj_list[j ^ 1].emplace_back(i);
    }

    void assign(int i) {
        add_clause(i, i);
    }

    void add_at_most_one(const vector<int> &literals) {
        if (literals.size() < 2) return;

        int prev = ~literals[0];
        for (int i = 2; i < literals.size(); i++) {
            int curr = add_variable();
            add_clause(prev, ~literals[i]);
            add_clause(prev, curr);
            add_clause(~literals[i], curr);
            prev = ~curr;
        }
        add_clause(prev, ~literals[1]);
    }

    pair<bool, vector<int>> solve() {
        deque<int> q;
        vector<int> assignment(n, -1);
        auto bfs = [&](int s) {
            q = {s};
            assignment[s >> 1] = !(s & 1);
            for (int i = 0; i < q.size(); i++) {
                int v = q[i];
                for (int u : adj_list[v])
                    if (assignment[u >> 1] == -1) {
                        assignment[u >> 1] = !(u & 1);
                        q.emplace_back(u);
                    } else if (assignment[u >> 1] == (u & 1)) return false;
            }
            return true;
        };

        for (int i = 0; i < n; i++) {
            if (assignment[i] != -1 || bfs(i << 1 | 1)) continue;
            for (int v : q) assignment[v >> 1] = -1;
            if (!bfs(i << 1)) return {false, {}};
        }
        return {true, assignment};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> U(m), V(m), L(m);
    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 0; i < m; i++) {
        cin >> U[i] >> V[i] >> L[i];
        U[i]--;
        V[i]--;
        L[i]--;

        adj_list[U[i]].emplace_back(V[i], i);
        adj_list[V[i]].emplace_back(U[i], i);
    }

    BlockCutTree bct(n, m, adj_list);
    int bccs = bct.bccs.size();
    vector<vector<int>> edges(bccs), adj_list_tree(n + bccs);
    for (int i = 0; i < m; i++) edges[bct.edge_component[i]].emplace_back(i);
    for (int i = 0; i < bccs; i++)
        for (int v : bct.bccs[i]) {
            adj_list_tree[v].emplace_back(i + n);
            adj_list_tree[i + n].emplace_back(v);
        }

    vector<int> parent(n + bccs, -1);
    parent[0] = 0;
    auto dfs = [&](auto &&self, int v = 0) -> void {
        for (int u : adj_list_tree[v])
            if (!~parent[u]) {
                parent[u] = v;
                self(self, u);
            }
    };
    dfs(dfs);

    vector<vector<pair<int, int>>> cycles(bccs);
    vector<array<int, 2>> incident(n);
    vector<int> degree(n, 0);
    for (int c = 0; c < bccs; c++)
        if (!bct.bridge(edges[c][0])) {
            for (int v : bct.bccs[c]) degree[v] = 0;

            for (int i : edges[c]) {
                incident[U[i]][degree[U[i]]++] = i;
                incident[V[i]][degree[V[i]]++] = i;
            }

            int v = bct.bccs[c][0], prev = -1;
            do {
                int e = incident[v][0] != prev ? incident[v][0] : incident[v][1];
                cycles[c].emplace_back(v, e);
                v = U[e] ^ V[e] ^ v;
                prev = e;
            } while (v != bct.bccs[c][0]);
        }

    vector<bool> on_path(n + bccs, false);
    on_path[0] = true;
    for (int v = 1; v; v = parent[v]) on_path[v] = true;

    auto pos = [&](int c, int v) {
        return find_if(cycles[c].begin(), cycles[c].end(), [&](auto p) { return p.first == v; }) - cycles[c].begin();
    };
    int variables = 0;
    vector<int> state(m, -1);
    for (int v = 1; v; v = parent[parent[v]]) {
        int c = parent[v] - n, u = parent[parent[v]];
        if (bct.bridge(edges[c][0])) {
            state[edges[c][0]] = -2;
            continue;
        }

        for (int i : edges[c]) state[i] = 2 * variables;

        int l = pos(c, u), r = pos(c, v);
        for (int k = l; k != r; ++k %= cycles[c].size()) state[cycles[c][k].second]++;
        variables++;
    }

    vector<array<int, 2>> required(m, {-1, -1});
    for (int i = 0; i < m; i++)
        if (i < L[i]) {
            for (int j : {i, L[i]}) {
                if (state[j] == -2) {
                    required[i][0] = -2;
                    break;
                }
                if (!~state[j]) continue;

                if (!~required[i][0]) required[i][0] = state[j];
                else if (required[i][0] / 2 != state[j] / 2) required[i][1] = state[j];
                else if (required[i][0] != state[j]) required[i][0] = -2;
            }
            required[L[i]] = required[i];
        }

    auto clause = [&](int c, int l, int r, int z = -1) -> vector<int> {
        vector<int> conditions{-2};

        for (int k = l; k != r; ++k %= cycles[c].size()) {
            auto [a, b] = required[cycles[c][k].second];
            if (a == -2) continue;

            int y = -1;
            for (int x : {a, b})
                if (x >= 0) {
                    if (z >= 0 && x / 2 == z / 2) {
                        if (x != z) {
                            y = -2;
                            break;
                        }
                    } else y = x;
                }
            if (y == -2) continue;
            if (!~y) return {};

            if (conditions[0] == -2) conditions[0] = y;
            else if (conditions[0] != y) {
                conditions.emplace_back(y);
                return conditions;
            }
        }
        return conditions;
    };

    auto satisfiable = [&](const auto &c1, const auto &c2) {
        if ((!c1.empty() && c1[0] == -2) || (!c2.empty() && c2[0] == -2)) return false;
        if (c1.empty() || c2.empty()) return true;

        vector<int> literals;
        for (auto &conditions : {c1, c2})
            for (int x : conditions) literals.emplace_back(x / 2);
        sort(literals.begin(), literals.end());
        literals.erase(unique(literals.begin(), literals.end()), literals.end());

        TwoSATSystem sat(literals.size());
        auto literal = [&](int x) {
            int v = lower_bound(literals.begin(), literals.end(), x / 2) - literals.begin();
            return x & 1 ? v : ~v;
        };

        for (auto &conditions : {c1, c2})
            if (conditions.size() == 1) sat.assign(literal(conditions[0]));
            else if (conditions.size() == 2) sat.add_clause(literal(conditions[0]), literal(conditions[1]));
        return sat.solve().first;
    };

    for (int v = 2; v && !on_path[v]; v = parent[parent[v]]) {
        int c = parent[v] - n, u = parent[parent[v]];
        if (bct.bridge(edges[c][0])) {
            if (!on_path[parent[v]] && required[edges[c][0]][0] != -2) {
                cout << "Ja";
                exit(0);
            }
            continue;
        }

        int l = pos(c, u), r = pos(c, v);
        if (!on_path[parent[v]]) {
            if (satisfiable(clause(c, l, r), clause(c, r, l))) {
                cout << "Ja";
                exit(0);
            }
            continue;
        }

        int z = state[edges[c][0]] / 2;
        for (int i = 0; i < 2; i++)
            if (satisfiable(clause(c, l, r, 2 * z + i), clause(c, r, l, 2 * z + i))) {
                cout << "Ja";
                exit(0);
            }
    }
    cout << "Nej";
}
