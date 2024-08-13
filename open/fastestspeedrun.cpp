#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<vector<int>, vector<long long>> edmonds(vector<vector<pair<int, int>>> adj_list_transpose) {
    int n = adj_list_transpose.size();

    DisjointSet dsu(n);
    vector<vector<long long>> dist(n, vector<long long>(n, 1e18));
    vector<int> state(n, 0);
    for (int v = 0; v < n; v++)
        for (auto [u, w] : adj_list_transpose[v]) dist[v][u] = min(dist[v][u], (long long) w);

    vector<int> prev(n);
    vector<long long> weights(n, 0);
    stack<int> path;
    state[0] = 2;
    for (int V = 0; V < n; V++) {
        if (state[dsu.find(V)] == 2) continue;

        int v = dsu.find(V);
        while (state[v] != 2)
            if (!state[v]) {
                for (int u = 0; u < n; u++)
                    if (dsu.find(u) == v) dist[v][u] = 1e18;

                int u = 0;
                auto w = dist[v][0];
                for (int t = 0; t < n; t++)
                    if (w > dist[v][t]) {
                        w = dist[v][t];
                        u = t;
                    }

                path.emplace(v);
                prev[v] = u;
                weights[v] = w;
                state[v] = 1;
                v = dsu.find(u);
            } else {
                while (path.top() != v) {
                    int u = path.top();
                    path.pop();

                    if (dsu.unite(v, u))
                        for (int t = 0; t < n; t++) dist[v][t] = min(dist[v][t], dist[u][t] + weights[v] - weights[u]);
                }

                v = dsu.find(v);
                state[v] = 0;
            }

        while (!path.empty()) {
            state[path.top()] = 2;
            path.pop();
        }
    }

    return {prev, weights};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<pair<int, int>>> adj_list_transpose(n + 1, vector<pair<int, int>>(n + 2));
    for (int i = 1; i <= n; i++) {
        int x, s;
        cin >> x >> s;

        adj_list_transpose[i][0] = {x, s};
        for (int j = 0; j <= n; j++) {
            int a;
            cin >> a;

            adj_list_transpose[i][j + 1] = {j, a};
        }
    }

    auto [prev, weights] = edmonds(adj_list_transpose);
    cout << accumulate(weights.begin(), weights.end(), 0LL, [&](auto d, auto w) {return d + w;});
}
