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
