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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, s, L;
    cin >> R >> s >> L;

    vector<pair<int, int>> edges(L);
    FlowNetwork<int> fn(L + R + s + 2);
    for (int i = 0; i < L; i++) {
        int a, b;
        cin >> a >> b;

        edges[i] = {a, b};
        fn.add_arc(0, i + 1, 1);
        fn.add_arc(i + 1, L + a, 1);
        fn.add_arc(i + 1, L + R + b, 1);
    }

    auto temp = fn;
    int l = 0, r = L, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        for (int i = 0; i < R; i++) fn.add_arc(L + i + 1, L + R + s + 1, m);
        for (int i = 0; i < s; i++) fn.add_arc(L + R + i + 1, L + R + s + 1, m);

        if (fn.max_flow(0, L + R + s + 1) == L) r = m;
        else l = m;

        fn = temp;
    }
    for (int i = 0; i < R; i++) fn.add_arc(L + i + 1, L + R + s + 1, r);
    for (int i = 0; i < s; i++) fn.add_arc(L + R + i + 1, L + R + s + 1, r);
    fn.max_flow(0, L + R + s + 1);

    vector<pair<string, int>> rulings(L);
    for (int i = 0; i < L; i++)
        for (auto [u, v, cap, _] : fn.network[i + 1])
            if (L < u && u <= L + R + s && !cap) {
                if (u <= L + R) rulings[i] = {"INDV", edges[i].first};
                else rulings[i] = {"CORP", edges[i].second};
                break;
            }

    for (auto [party, i] : rulings) cout << party << " " << i << "\n";
}
