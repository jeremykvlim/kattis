#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<vector<int>, vector<long long>> edmonds(const vector<vector<pair<int, int>>> &adj_list_transpose) {
    int n = adj_list_transpose.size();

    vector<vector<long long>> dist(n, vector<long long>(n, 1e18));
    for (int v = 0; v < n; v++)
        for (auto [u, w] : adj_list_transpose[v]) dist[v][u] = min(dist[v][u], (long long) w);

    DisjointSets dsu(n);
    stack<int> path;
    vector<int> state(n, 0), prev(n);
    state[0] = 2;
    vector<long long> weights(n, 0);
    for (int i = 0; i < n; i++) {
        if (state[dsu.find(i)] == 2) continue;

        int v = dsu.find(i);
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

    vector<vector<pair<int, int>>> adj_list_transpose(n + 1);
    for (int i = 1; i <= n; i++) {
        int x, s;
        cin >> x >> s;

        for (int j = 0; j <= n; j++) {
            int a;
            cin >> a;

            if (i != j) adj_list_transpose[i].emplace_back(j, j == x ? s : a);
        }
    }

    auto [prev, weights] = edmonds(adj_list_transpose);
    cout << accumulate(weights.begin(), weights.end(), 0LL);
}
