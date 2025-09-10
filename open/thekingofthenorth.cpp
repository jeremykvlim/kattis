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

    int R, C;
    cin >> R >> C;

    int most = -1;
    vector<vector<int>> m(R, vector<int>(C));
    for (auto &row : m)
        for (int &ci : row) {
            cin >> ci;

            most = max(most, ci);
        }

    int r, c;
    cin >> r >> c;

    FlowNetwork<int> fn(2 * R * C + 1);
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++) {
            fn.add_arc(i * C + j, i * C + j + R * C, m[i][j]);
            if (i) fn.add_arc(i * C + j + R * C, (i - 1) * C + j, most);
            if (j) fn.add_arc(i * C + j + R * C, i * C + j - 1, most);
            if (i + 1 < R) fn.add_arc(i * C + j + R * C, (i + 1) * C + j, most);
            if (j + 1 < C) fn.add_arc(i * C + j + R * C, i * C + j + 1, most);
            if (!i || !j || i + 1 == R || j + 1 == C) fn.add_arc(i * C + j + R * C, 2 * R * C, most);
        }
    cout << fn.max_flow(r * C + c, 2 * R * C);
}
