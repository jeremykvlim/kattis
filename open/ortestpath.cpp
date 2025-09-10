#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

template <typename T>
struct FlowNetwork {
    struct Arc {
        int u, rev;
        T cap, initial_cap;
        Arc(int u, int rev, T cap) : u(u), rev(rev), cap(cap), initial_cap(cap) {}
    };

    int n;
    vector<vector<Arc>> network;
    vector<int> dist;
    vector<typename vector<Arc>::iterator> it;

    FlowNetwork(int n) : n(n), network(n), dist(n), it(n) {}

    void add_arc(int u, int v, T cap_uv, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu);
    }

    bool bfs(int s, int t) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, _, cap, __] : network[v])
                if (cap > 0 && !~dist[u]) {
                    dist[u] = dist[v] + 1;
                    q.emplace(u);
                }
        }
        return ~dist[t];
    }

    T dfs(int v, int t, T flow) {
        if (v == t) return flow;

        for (; it[v] != network[v].end(); it[v]++) {
            auto &[u, rev, cap, _] = *it[v];
            if (cap > 0 && dist[u] == dist[v] + 1) {
                T f = dfs(u, t, min(flow, cap));
                if (f > 0) {
                    cap -= f;
                    network[u][rev].cap += f;
                    return f;
                }
            }
        }
        return (T) 0;
    }

    T max_flow(int s, int t) {
        T flow = 0, f;
        while (bfs(s, t)) {
            for (int v = 0; v < n; v++) it[v] = network[v].begin();
            while ((f = dfs(s, t, numeric_limits<T>::max())) > 0) flow += f;
        }
        return flow;
    }

    vector<tuple<int, int, T>> flow_decomposition() {
        vector<tuple<int, int, T>> path;
        for (int v = 0; v < n; v++)
            for (auto [u, _, cap, initial_cap] : network[v])
                if (cap > 0 && cap > initial_cap) path.emplace_back(u, v, cap - initial_cap);

        return path;
    }
};

struct BlockCutTree {
    int n;
    vector<vector<int>> bccs;
    vector<int> edge_component;
    vector<bool> cutpoint;

    BlockCutTree(int n, int m, vector<vector<pair<int, int>>> &adj_list) : n(n), cutpoint(n, false), edge_component(m) {
        tarjan(adj_list);
    };

    void tarjan(vector<vector<pair<int, int>>> &adj_list) {
        vector<int> order(n, 0), low(n, 0);
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
        for (int v = 0; v < n; v++)
            if (!order[v]) dfs(dfs, v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n);
    unordered_set<pair<int, int>, Hash> trues;
    for (int i = 0; i < m; i++) {
        int u, v;
        bool b;
        cin >> u >> v >> b;

        edges[i] = minmax(u, v);
        adj_list[u].emplace_back(v, i);
        adj_list[v].emplace_back(u, i);
        if (b) trues.emplace(edges[i]);
    }

    BlockCutTree bct(n, m, adj_list);
    vector<pair<int, int>> bcc_true(bct.bccs.size(), {-1, -1});
    unordered_map<pair<int, int>, int, Hash> components;
    for (int i = 0; i < m; i++) {
        components[edges[i]] = bct.edge_component[i];
        if (bcc_true[bct.edge_component[i]] == make_pair(-1, -1) && trues.count(edges[i])) bcc_true[bct.edge_component[i]] = edges[i];
    }

    vector<int> prev(n, -1);
    vector<bool> visited(n, false);
    visited[s] = true;
    queue<int> q;
    q.emplace(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, i] : adj_list[v])
            if (!visited[u]) {
                visited[u] = true;
                prev[u] = v;
                q.emplace(u);
            }
    }
    if (!visited[t]) {
        cout << -1;
        exit(0);
    }

    vector<int> path;
    for (int v = t; ~v; v = prev[v]) path.emplace_back(v);
    reverse(path.begin(), path.end());

    int c = -1;
    for (int i = 0; i < path.size() - 1; i++) {
        auto [u, v] = minmax(path[i], path[i + 1]);
        if (components.count({u, v})) {
            int comp = components[{u, v}];
            if (bcc_true[comp] != make_pair(-1, -1)) {
                c = comp;
                break;
            }
        }
    }
    if (!~c) {
        cout << -1;
        exit(0);
    }

    FlowNetwork<int> fn(2 * n + 2);
    for (int v = 0; v < n; v++) fn.add_arc(v, v + n, 1);
    for (auto [u, v] : edges) {
        fn.add_arc(u + n, v, 1);
        fn.add_arc(v + n, u, 1);
    }
    fn.add_arc(s + n, 2 * n + 1, 1);
    fn.add_arc(t + n, 2 * n + 1, 1);
    fn.add_arc(2 * n, bcc_true[c].first, 1);
    fn.add_arc(2 * n, bcc_true[c].second, 1);
    fn.max_flow(2 * n, 2 * n + 1);
    auto flow_path = fn.flow_decomposition();

    vector<vector<int>> adj_list_flow(2 * n + 2);
    for (auto [u, v, f] : flow_path)
        if (f > 0) adj_list_flow[u].emplace_back(v);

    vector<vector<int>> seqs(2 * n + 2);
    for (int u : adj_list_flow[2 * n])
        for (int v = u; v != 2 * n + 1 && !adj_list_flow[v].empty(); v = adj_list_flow[v][0])
            if (v < n) seqs[u].emplace_back(v);

    int x = adj_list_flow[2 * n][0], y = adj_list_flow[2 * n][1];
    if (find(seqs[x].begin(), seqs[x].end(), s) != seqs[x].end()) goto found;
    swap(x, y);

    found:;
    reverse(seqs[x].begin(), seqs[x].end());
    for (int v : seqs[x]) cout << v << " ";
    for (int v : seqs[y]) cout << v << " ";
}
