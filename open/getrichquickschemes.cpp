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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n), m(n);
    for (int i = 0; i < n; i++) cin >> p[i] >> m[i];

    int s;
    cin >> s;

    auto total = 0LL;
    vector<int> l(s);
    vector<vector<int>> stores(s);
    for (int i = 0; i < s; i++) {
        int a;
        cin >> l[i] >> a;

        total += l[i];
        stores[i].resize(a);
        for (int &c : stores[i]) {
            cin >> c;
            c--;
        }
    }

    FlowNetwork<long long> fn(n + s + 2);
    for (int i = 0; i < s; i++) fn.add_arc(0, i + 1, l[i]);
    for (int i = 0; i < s; i++)
        for (int c : stores[i]) fn.add_arc(i + 1, c + s + 1, total);

    vector<pair<int, int>> edges(n);
    for (int i = 0; i < n; i++) {
        fn.add_arc(i + s + 1, n + s + 1, 0);
        edges[i] = {i + s + 1, fn.network[i + s + 1].size() - 1};
    }

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return p[i] != p[j] ? p[i] > p[j] : i < j; });

    auto money = 0.;
    for (int i : indices) {
        auto [v, ei] = edges[i];
        fn.network[v][ei].cap += m[i];
        money += fn.max_flow(0, n + s + 1) * (p[i] / 1e2);
    }
    cout << fixed << setprecision(6) << money;
}
