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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> p(n + 1), q(n + 1);
    for (int i = 1; i <= n; i++) cin >> p[i];
    for (int i = 1; i <= n; i++) cin >> q[i];

    vector<int> L(m + 1), R(m + 1), x(m + 1);
    for (int i = 1; i <= m; i++) cin >> L[i] >> R[i] >> x[i];

    auto sum = 0LL;
    vector<long long> pref_p(n + 1), pref_diff(n + 1);
    for (int i = 1; i <= n; i++) {
        pref_p[i] = pref_p[i - 1] + p[i];
        pref_diff[i] = pref_diff[i - 1] + q[i] - p[i];
        sum += (long long) p[i] * q[i];
    }

    FlowNetwork<long long> flow(m + 2);
    for (int i = 1; i <= m; i++) {
        sum += (long long) x[i] * (pref_p[R[i]] - pref_p[L[i] - 1]);
        auto c = (long long) x[i] * (pref_diff[R[i]] - pref_diff[L[i] - 1]);
        if (c >= 0) flow.add_arc(0, i, c);
        else {
            flow.add_arc(i, m + 1, -c);
            sum += c;
        }
    }

    for (int i = 1; i <= m; i++)
        for (int j = i + 1; j <= m; j++) {
            int l = max(L[i], L[j]), r = min(R[i], R[j]);
            if (l <= r) {
                auto c = (long long) x[i] * x[j] * (r - l + 1);
                flow.add_arc(i, j, c, c);
            }
        }
    cout << sum + flow.max_flow(0, m + 1);
}