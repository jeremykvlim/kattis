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

    vector<int> min_cut(int s, const vector<array<int, 3>> &edges) {
        vector<bool> visited(n, false);
        visited[s] = true;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, _, cap, __] : network[v])
                if (cap > 0 && !visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
        }

        vector<int> cut_edges;
        for (auto [u, v, i] : edges)
            if (visited[u] && !visited[v]) cut_edges.emplace_back(i);
        return cut_edges;
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
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [di, v] = pq.top();
        pq.pop();

        if (di > dist[v]) continue;

        for (auto [u, w] : adj_list[v])
            if (dist[u] > di + w) {
                dist[u] = di + w;
                pq.emplace(dist[u], u);
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

    auto cut_edges = fn.min_cut(s, sp_edges);
    cout << cut_edges.size() << "\n";
    for (int i : cut_edges) cout << roads[i][0] << " " << roads[i][1] << " " << 1 << "\n";
}
