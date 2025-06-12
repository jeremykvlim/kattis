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
    FlowNetwork(int n) : n(n), network(n) {}

    void add_arc(int u, int v, T cap_uv, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu);
    }

    T max_flow(int s, int t) {
        if (s == t) return 0;

        vector<T> excess(n, 0);
        vector<int> height(n, 0), count(2 * n, 0);
        vector<typename vector<Arc>::iterator> curr(n);
        excess[t] = 1;
        height[s] = n;
        count[0] = n - 1;
        for (int v = 0; v < n; v++) curr[v] = network[v].begin();
        vector<stack<int>> active_stacks(2 * n);

        auto push = [&](int v, Arc &a, T delta) {
            int u = a.u;
            if (!excess[u] && delta) active_stacks[height[u]].emplace(u);
            a.cap -= delta;
            network[u][a.rev].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;
        };

        auto relabel = [&](int v, int h) {
            if (h < height[v] && height[v] < n) {
                height[v] = n + 1;
                count[height[v]]--;
            }
        };

        for (auto &&a : network[s]) push(s, a, a.cap);

        auto discharge = [&](int v, int &h) {
            while (excess[v] > 0)
                if (curr[v] == network[v].end()) {
                    int hv = height[v];
                    height[v] = INT_MAX;

                    for (auto a = network[v].begin(); a != network[v].end(); a++)
                        if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                    count[height[v]]++;
                    if (!--count[hv] && hv < n)
                        for (int u = 0; u < n; u++) relabel(u, hv);
                } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                else curr[v]++;

            if (h) h--;
            while (h < 2 * n && active_stacks[h].empty()) h++;
        };

        if (!active_stacks[0].empty())
            for (int h = 0; h < 2 * n;) {
                int v = active_stacks[h].top();
                active_stacks[h].pop();

                discharge(v, h);
            }

        return -excess[s];
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
    vector<vector<pair<int, int>>> bcc_edges;
    vector<bool> cutpoint;

    BlockCutTree(int n, vector<vector<int>> &adj_list) : n(n), cutpoint(n, false) {
        tarjan(adj_list);
        bcc_edges.resize(bccs.size());
        vector<bool> member(n, false);
        for (int c = 0; c < bccs.size(); c++) {
            for (int v : bccs[c]) member[v] = true;

            for (int v : bccs[c])
                for (int u : adj_list[v])
                    if (member[u] && v < u) bcc_edges[c].emplace_back(v, u);
            for (int v : bccs[c]) member[v] = false;
        }
    };

    void tarjan(vector<vector<int>> &adj_list) {
        vector<int> order(n, 0), low(n, 0);
        stack<int> st;
        int count = 0;

        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            order[v] = low[v] = ++count;
            st.emplace(v);
            for (int u : adj_list[v])
                if (u != prev) {
                    if (!order[u]) {
                        self(self, u, v);
                        low[v] = min(low[v], low[u]);

                        if (low[u] >= order[v]) {
                            cutpoint[v] = (order[v] > 1 || order[u] > 2);
                            bccs.emplace_back(vector<int>{v});

                            while (bccs.back().back() != u) {
                                bccs.back().emplace_back(st.top());
                                st.pop();
                            }
                        }
                    } else low[v] = min(low[v], order[u]);
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
    vector<vector<int>> adj_list(n);
    unordered_set<pair<int, int>, Hash> trues;
    for (int i = 0; i < m; i++) {
        int u, v;
        bool b;
        cin >> u >> v >> b;

        edges[i] = {u, v};
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
        if (b) trues.emplace(minmax(u, v));
    }

    BlockCutTree bct(n, adj_list);
    vector<pair<int, int>> bcc_true(bct.bccs.size(), {-1, -1});
    unordered_map<pair<int, int>, int, Hash> indices;
    for (int i = 0; i < bct.bccs.size(); i++)
        for (auto [u, v] : bct.bcc_edges[i]) {
            indices[{u, v}] = i;
            if (bcc_true[i] == make_pair(-1, -1) && trues.count({u, v})) bcc_true[i] = {u, v};
        }

    vector<int> prev(n, -1);
    vector<bool> visited(n, false);
    visited[s] = true;
    queue<int> q;
    q.emplace(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list[v])
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
        if (indices.count({u, v})) {
            int comp = indices[{u, v}];
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
