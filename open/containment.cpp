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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    FlowNetwork<int> fn(1e3 + 2);
    int s = 1e3, t = 1e3 + 1;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            for (int k = 0; k < 10; k++) {
                int u = i * 100 + j * 10 + k;
                fn.add_arc(u, i ? (i - 1) * 100 + j * 10 + k : t, 1);
                fn.add_arc(u, i < 9 ? (i + 1) * 100 + j * 10 + k : t, 1);
                fn.add_arc(u, j ? i * 100 + (j - 1) * 10 + k : t, 1);
                fn.add_arc(u, j < 9 ? i * 100 + (j + 1) * 10 + k : t, 1);
                fn.add_arc(u, k ? i * 100 + j * 10 + (k - 1) : t, 1);
                fn.add_arc(u, k < 9 ? i * 100 + j * 10 + (k + 1) : t, 1);
            }

    while (n--) {
        int x, y, z;
        cin >> x >> y >> z;

        fn.add_arc(s, x * 100 + y * 10 + z, 6);
    }
    cout << fn.max_flow(s, t);
}
