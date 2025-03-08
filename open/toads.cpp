#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

struct DisjointSets {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

vector<int> tarjan_lca(int n, int q, const vector<vector<int>> &adj_list, const vector<vector<pair<int, int>>> &queries) {
    DisjointSets dsu(n);
    vector<bool> visited(n, false);
    vector<int> lca(q, -1), ancestor(n, -1);

    auto dfs = [&](auto &&self, int v = 0) -> void {
        visited[v] = true;
        ancestor[v] = v;

        for (int u : adj_list[v])
            if (!visited[u]) {
                self(self, u);
                dsu.unite(v, u);
                ancestor[dsu.find(v)] = v;
            }

        for (auto &[u, i] : queries[v])
            if (visited[u]) lca[i] = ancestor[dsu.find(u)];
    };
    dfs(dfs);
    return lca;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<unordered_map<int, int>> adj_matrix(n);
    unordered_set<pair<int, int>, Hash> edges;
    auto add_edge = [&](int u, int v, int w) {
        if (!adj_matrix[u].count(v)) adj_matrix[u][v] = w;
        else adj_matrix[u][v] = min(adj_matrix[u][v], w);

        if (!adj_matrix[v].count(u)) adj_matrix[v][u] = w;
        else adj_matrix[v][u] = min(adj_matrix[v][u], w);

        edges.emplace(minmax(u, v));
    };

    for (int i = 0; i < n; i++) {
        int j, t;
        cin >> j >> t;
        j--;

        add_edge(i, j, t);
    }

    int l;
    cin >> l;

    while (l--) {
        int x, y, t;
        cin >> x >> y >> t;
        x--;
        y--;

        add_edge(x, y, t);
    }

    int q;
    cin >> q;

    vector<pair<int, int>> queries(q);
    for (auto &[a, b] : queries) {
        cin >> a >> b;
        a--;
        b--;
    }

    int count = 0;
    vector<int> component(n, -1), c_indices(n, -1);
    vector<vector<int>> components;
    vector<vector<vector<int>>> c_adj_lists;
    vector<long long> t(n, 0);
    for (int i = 0; i < n; i++) {
        if (~component[i]) continue;

        vector<int> c;
        vector<pair<int, int>> c_edges;
        stack<pair<int, int>> st;
        st.emplace(i, -1);
        while (!st.empty()) {
            auto [v, p] = st.top();
            st.pop();

            if (~component[v]) continue;

            component[v] = count;
            c.emplace_back(v);
            c_indices[v] = c.size() - 1;
            if (p != -1) {
                c_edges.emplace_back(c_indices[p], c_indices[v]);
                t[v] = t[p] + adj_matrix[p][v];
                edges.erase(minmax(p, v));
            }

            for (auto [u, w] : adj_matrix[v]) st.emplace(u, v);
        }

        int s = c.size();
        vector<vector<int>> c_adj_list(s);
        for (auto [u, v] : c_edges) {
            c_adj_list[u].emplace_back(v);
            c_adj_list[v].emplace_back(u);
        }

        count++;
        components.emplace_back(c);
        c_adj_lists.emplace_back(c_adj_list);
    }
    vector<long long> time(q, 0);
    vector<vector<int>> c_query_indices(count), leftover(count);
    for (int i = 0; i < q; i++) {
        auto [a, b] = queries[i];
        if (component[a] == component[b]) c_query_indices[component[a]].emplace_back(i);
        else time[i] = -1;
    }
    for (auto [u, v] : edges) leftover[component[u]].emplace_back(u);

    for (int c = 0; c < count; c++) {
        int s = components[c].size();

        vector<vector<pair<int, int>>> lca_queries(s);
        for (int qi : c_query_indices[c]) {
            auto [a, b] = queries[qi];
            lca_queries[c_indices[a]].emplace_back(c_indices[b], qi);
            lca_queries[c_indices[b]].emplace_back(c_indices[a], qi);
        }
        auto lca = tarjan_lca(s, q, c_adj_lists[c], lca_queries);
        for (int qi : c_query_indices[c]) {
            auto [a, b] = queries[qi];
            time[qi] = t[a] + t[b] - 2 * t[components[c][lca[qi]]];
        }

        if (!leftover[c].empty()) {
            vector<vector<pair<long long, int>>> dist(s);
            priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>, greater<tuple<long long, int, int>>> pq;
            for (int x : leftover[c]) {
                dist[c_indices[x]].emplace_back(0, x);
                pq.emplace(0, c_indices[x], x);
            }

            while (!pq.empty()) {
                auto [d, v, x1] = pq.top();
                pq.pop();

                for (auto [dv, x2] : dist[v])
                    if (d == dv && x1 == x2) goto valid;
                continue;
                
                valid:;
                for (auto [y, w] : adj_matrix[components[c][v]]) {
                    if (component[y] != c) continue;

                    int u = c_indices[y];
                    for (auto &[du, x2] : dist[u])
                        if (x1 == x2) {
                            if (du > d + w) {
                                du = d + w;
                                pq.emplace(du, u, x1);
                            }
                            goto next;
                        }

                    dist[u].emplace_back(d + w, x1);
                    pq.emplace(d + w, u, x1);
                    next:;
                }
            }

            for (int i : c_query_indices[c]) {
                auto [a, b] = queries[i];
                for (auto [d1, x1] : dist[c_indices[a]])
                    for (auto [d2, x2] : dist[c_indices[b]])
                        if (x1 == x2) time[i] = min(time[i], d1 + d2);
            }
        }
    }

    for (auto ti : time) cout << ti << "\n";
}
