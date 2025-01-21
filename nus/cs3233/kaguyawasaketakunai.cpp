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
            network[u][a.v].cap += delta;
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
                    height[v] = INT_MAX;

                    for (auto a = network[v].begin(); a != network[v].end(); a++)
                        if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                    count[height[v]]++;
                    if (!--count[h] && h < n)
                        for (int u = 0; u < n; u++) relabel(u, h);
                    h = height[v];
                } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                else curr[v]++;

            while (h >= 0 && active_stacks[h].empty()) h--;
        };

        if (!active_stacks[0].empty())
            for (int h = 0; h >= 0;) {
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, e, s, d;
    cin >> n >> e >> s >> d;

    vector<vector<pair<int, long long>>> adj_list(n + 1);
    vector<array<int, 3>> roads(e);
    for (auto &[u, v, w] : roads) {
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<long long> dist(n + 1, 1e18);
    dist[s] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [di, u] = pq.top();
        pq.pop();

        if (di > dist[u]) continue;

        for (auto [v, w] : adj_list[u])
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
    }

    FlowNetwork<int> fn(n + 1);
    vector<array<int, 3>> sp_edges;
    for (int i = 0; i < e; i++) {
        auto [u, v, w] = roads[i];
        if (dist[u] + w == dist[v]) {
            fn.add_arc(u, v, 1);
            sp_edges.push_back({u, v, i});
        }
        if (dist[v] + w == dist[u]) {
            fn.add_arc(v, u, 1);
            sp_edges.push_back({v, u, i});
        }
    }
    fn.max_flow(s, d);

    vector<bool> visited(n + 1, false);
    visited[s] = true;
    queue<int> q;
    q.emplace(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, _, cap, __] : fn.network[v])
            if (cap > 0 && !visited[u]) {
                visited[u] = true;
                q.emplace(u);
            }
    }

    vector<int> cut_edges;
    for (auto [u, v, i] : sp_edges)
        if (visited[u] && !visited[v]) cut_edges.emplace_back(i);

    cout << cut_edges.size() << "\n";
    for (int i : cut_edges) cout << roads[i][0] << " " << roads[i][1] << " " << 1 << "\n";
}
