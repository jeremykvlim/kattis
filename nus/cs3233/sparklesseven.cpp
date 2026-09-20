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

    vector<string> names{"Twilight Sparkle", "Applejack", "Rarity", "Pinkie Pie", "Fluttershy", "Rainbow Dash", "Spike"};

    vector<int> adj_masks(7, 0);
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            bool b;
            cin >> b;

            if (b) adj_masks[i] |= 1 << j;
        }

    for (int p = 1; p <= 7; p++) {
        FlowNetwork<int> fn(16);
        for (int i = 0; i < 7; i++) {
            fn.add_arc(0, i + 1, 1);
            fn.add_arc(i + 8, 15, p);
        }

        for (int i = 0; i < 7; i++)
            for (int j = 0; j < 7; j++)
                if ((adj_masks[i] >> j) & 1) fn.add_arc(j + 1, i + 8, 1);

        if (fn.max_flow(0, 15) != 7) continue;

        vector<vector<int>> heist(7);
        for (auto [u, v, f] : fn.flow_decomposition())
            if (1 <= u && u < 8 && 8 <= v && v < 15) heist[v - 8].emplace_back(u);

        cout << p << "\n";
        for (int op = 0; op < p; op++) {
            int m = 0;
            for (int i = 0; i < 7; i++)
                if (op < heist[i].size()) m++;

            cout << m << "\n";
            for (int i = 0; i < 7; i++)
                if (op < heist[i].size()) cout << names[i] << " " << heist[i][op] << "\n";
        }
        exit(0);
    }
    cout << "IMPOSSIBLE";
}
