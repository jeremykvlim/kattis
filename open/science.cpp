#include <bits/stdc++.h>
using namespace std;

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

    T dfs(int v, int t, T f) {
        if (v == t) return f;

        for (; it[v] != network[v].end(); it[v]++) {
            auto &[u, rev, cap, _] = *it[v];
            if (cap > 0 && dist[u] == dist[v] + 1) {
                T aug = dfs(u, t, min(f, cap));
                if (aug > 0) {
                    cap -= aug;
                    network[u][rev].cap += aug;
                    return aug;
                }
            }
        }
        return 0;
    }

    T max_flow(int s, int t) {
        T flow = 0, aug;
        while (bfs(s, t)) {
            for (int v = 0; v < n; v++) it[v] = network[v].begin();
            while ((aug = dfs(s, t, numeric_limits<T>::max())) > 0) flow += aug;
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

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> buttons(n);
    for (auto &line : buttons) cin >> line;

    FlowNetwork<int> fn(2 * n + 2);
    vector<int> degree_r(n, 0), degree_c(n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (buttons[i][j] == 'Y') {
                fn.add_arc(i, j + n, 1);
                degree_r[i]++;
                degree_c[j]++;
            }

    auto temp = fn;
    int l = 0, r = min(*min_element(degree_r.begin(), degree_r.end()), *min_element(degree_c.begin(), degree_c.end())) + 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        for (int i = 0; i < n; i++) {
            fn.add_arc(2 * n, i, m);
            fn.add_arc(i + n, 2 * n + 1, m);
        }

        if (fn.max_flow(2 * n, 2 * n + 1) == m * n) l = m;
        else r = m;

        fn = temp;
    }

    int k = l;
    cout << k << "\n";
    if (!k) exit(0);

    for (int i = 0; i < n; i++) {
        fn.add_arc(2 * n, i, k);
        fn.add_arc(i + n, 2 * n + 1, k);
    }
    fn.max_flow(2 * n, 2 * n + 1);

    vector<vector<bool>> adj_matrix(n, vector<bool>(n, false));
    auto path = fn.flow_decomposition();
    for (auto [u, v, f] : path)
        if (0 <= u && u < n && n <= v && v < 2 * n && f) adj_matrix[u][v - n] = true;

    while (k--) {
        vector<pair<int, int>> edges;
        for (int u = 0; u < n; u++)
            for (int v = 0; v < n; v++)
                if (adj_matrix[u][v]) edges.emplace_back(u, v);

        auto [l, r, matches] = hopcroft_karp(n, n, edges);

        for (int v = 0; v < n; v++) {
            cout << r[v] + 1 << " ";
            adj_matrix[r[v]][v] = false;
        }
        cout << "\n";
    }
}
