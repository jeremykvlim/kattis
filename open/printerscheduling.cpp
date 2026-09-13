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

    int T;
    cin >> T;

    while (T--) {
        int n, m;
        cin >> n >> m;

        int sum = 0;
        vector<int> p(n), r(n), d(n), times(2 * n);
        for (int i = 0; i < n; i++) {
            cin >> p[i] >> r[i] >> d[i];

            sum += p[i];
            times[2 * i] = r[i];
            times[2 * i + 1] = d[i];
        }
        sort(times.begin(), times.end());
        times.erase(unique(times.begin(), times.end()), times.end());

        int t = times.size() - 1;
        FlowNetwork<int> fn(t + n + 2);
        for (int i = 0; i < n; i++) fn.add_arc(0, i + 1, p[i]);
        for (int i = 0; i < t; i++) fn.add_arc(i + n + 1, t + n + 1, m * (times[i + 1] - times[i]));

        vector<vector<int>> edge(n, vector<int>(t, -1));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < t; j++)
                if (r[i] <= times[j] && times[j + 1] <= d[i]) {
                    fn.add_arc(i + 1, j + n + 1, times[j + 1] - times[j]);
                    edge[i][j] = fn.network[i + 1].size() - 1;
                }

        if (fn.max_flow(0, t + n + 1) != sum) {
            cout << "NO\n\n";
            continue;
        }

        vector<vector<array<int, 3>>> files(n);
        for (int j = 0; j < t; j++)
            for (int i = 0, x = times[j], y = 0, z = 1; i < n; i++)
                if (~edge[i][j]) {
                    auto [u, _, cap, initial_cap] = fn.network[i + 1][edge[i][j]];
                    for (int c = initial_cap - cap; c;) {
                        y = x + min(c, times[j + 1] - x);
                        files[i].push_back({x, y, z});
                        c -= y - x;
                        if (y == times[j + 1]) {
                            x = times[j];
                            z++;
                        } else x = y;
                    }
                }

        cout << "YES\n";
        for (auto &f : files) {
            cout << f.size() << "\n";
            for (auto [x, y, z] : f) cout << x << " " << y << " " << z << "\n";
        }
        cout << "\n";
    }
}
