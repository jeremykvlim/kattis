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

    int L, E;
    cin >> L >> E;

    auto apples = 0LL;
    vector<long long> strength_a(E), count_a(E);
    unordered_map<string, int> compress;
    for (int i = 0; i < E; i++) {
        string name;
        cin >> name >> strength_a[i] >> count_a[i];

        apples += count_a[i];
        compress[name] = i;
    }

    auto doctors = 0LL;
    vector<long long> strength_d(L), count_d(L);
    vector<vector<int>> resistant(L);
    for (int i = 0; i < L; i++) {
        string name;
        int k;
        cin >> name >> strength_d[i] >> count_d[i] >> k;

        doctors += count_d[i];
        resistant[i].resize(k);
        for (int &apple : resistant[i]) {
            cin >> name;

            apple = compress[name];
        }
        sort(resistant[i].begin(), resistant[i].end());
    }

    vector<vector<int>> adj_list(E);
    for (int i = 0; i < L; i++)
        for (int j = 0; j < E; j++)
            if (!binary_search(resistant[i].begin(), resistant[i].end(), j) && strength_a[j] >= strength_d[i]) adj_list[j].emplace_back(i);

    long long l = 0, r = apples / doctors + 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        FlowNetwork<long long> fn(L + E + 2);
        for (int j = 0; j < E; j++) {
            fn.add_arc(0, j + 1, count_a[j]);
            for (int i : adj_list[j]) fn.add_arc(j + 1, i + E + 1, 1e18);
        }

        auto total = 0LL;
        for (int i = 0; i < L; i++) {
            auto cap = count_d[i] * m;
            total += cap;
            fn.add_arc(i + E + 1, L + E + 1, cap);
        }

        if (fn.max_flow(0, L + E + 1) == total) l = m;
        else r = m;
    }
    cout << l;
}
