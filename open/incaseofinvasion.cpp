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

    int n, m, S;
    cin >> n >> m >> S;

    auto total = 0LL;
    vector<long long> p(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> p[i];

        total += p[i];
    }

    if (!total) {
        cout << 0;
        exit(0);
    }

    vector<vector<pair<int, long long>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<int> s(S);
    vector<long long> c(S);
    for (int i = 0; i < S; i++) cin >> s[i] >> c[i];

    vector<vector<long long>> dist(S, vector<long long>(n + 1, 1e18));
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int i = 0; i < S; i++) {
        auto &d = dist[i];
        d[s[i]] = 0;
        pq.emplace(0, s[i]);

        while (!pq.empty()) {
            auto [dv, v] = pq.top();
            pq.pop();

            if (dv != d[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (d[u] > dv + w) {
                    d[u] = dv + w;
                    pq.emplace(d[u], u);
                }
        }
    }

    auto possible = [&](long long mid) -> bool {
        vector<long long> people(1 << S, 0);
        for (int j = 1; j <= n; j++)
            if (p[j]) {
                int mask = 0;
                for (int i = 0; i < S; i++)
                    if (dist[i][j] <= mid) mask |= 1 << i;
                if (!mask) return false;
                people[mask] += p[j];
            }

        vector<int> masks;
        for (int mask = 1; mask < 1 << S; mask++)
            if (people[mask]) masks.emplace_back(mask);

        int m = masks.size();
        FlowNetwork<long long> fn(S + m + 2);
        for (int j = 0; j < m; j++) {
            fn.add_arc(0, j + 1, people[masks[j]]);
            for (int i = 0; i < S; i++)
                if ((masks[j] >> i) & 1) fn.add_arc(j + 1, i + m + 1, 1e18);
        }
        for (int i = 0; i < S; i++) fn.add_arc(i + m + 1, S + m + 1, c[i]);
        return fn.max_flow(0, S + m + 1) == total;
    };

    long long l = -1, r = 0, mid;
    for (int i = 0; i < S; i++)
        for (int j = 1; j <= n; j++) r = max(r, dist[i][j]);
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        if (possible(mid)) r = mid;
        else l = mid;
    }
    cout << r;
}
