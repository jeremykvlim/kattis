#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FlowNetwork {
    struct Arc {
        int u, v;
        T cap, initial_cap;
        Arc(int u, int v, T cap) : u(u), v(v), cap(cap), initial_cap(cap) {}
    };

    int n;
    vector<vector<Arc>> network;
    vector<bool> cut;
    FlowNetwork(int n) : n(n), network(n) {}

    void add_arc(int u, int v, T cap_uv, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu);
    }

    T max_flow(int s, int t) {
        cut.assign(n, false);

        if (s == t) return 0;

        vector<T> excess(n, 0);
        vector<int> height(n, 0), height_count(2 * n, 0);
        vector<typename vector<Arc>::iterator> curr(n);
        excess[t] = 1;
        height[s] = n;
        height_count[0] = n - 1;
        for (int v = 0; v < n; v++) curr[v] = network[v].begin();

        vector<vector<int>> active(2 * n);
        auto push = [&](int v, Arc &e, T delta) {
            int u = e.u;
            if (!abs(excess[u]) && delta > 0) active[height[u]].emplace_back(u);
            e.cap -= delta;
            network[u][e.v].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;
        };

        for (auto &&e : network[s]) push(s, e, e.cap);

        if (!active[0].empty())
            for (int h = 0; h >= 0;) {
                int v = active[h].back();
                active[h].pop_back();

                while (excess[v] > 0)
                    if (curr[v] == network[v].end()) {
                        height[v] = INT_MAX;

                        for (auto e = network[v].begin(); e != network[v].end(); e++)
                            if (e->cap > 0 && height[v] > height[e->u] + 1) height[v] = height[(curr[v] = e)->u] + 1;

                        height_count[height[v]]++;
                        if (!--height_count[h] && h < n)
                            for (int w = 0; w < n; w++)
                                if (h < height[w] && height[w] < n) {
                                    height_count[height[w]]--;
                                    height[w] = n + 1;
                                }
                        h = height[v];
                    } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                    else curr[v]++;

                while (h >= 0 && active[h].empty()) h--;
            }

        for (int v = 0; v < n; v++) cut[v] = height[v] >= n;
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    FlowNetwork<int> fn(n);
    while (m--) {
        int u, v, c;
        cin >> u >> v >> c;

        fn.add_arc(u, v, c);
    }

    cout << n << " " << fn.max_flow(s, t) << " ";
    auto path = fn.flow_decomposition();
    cout << path.size() << "\n";
    for (auto [u, v, x] : path) cout << u << " " << v << " " << x << "\n";
}
