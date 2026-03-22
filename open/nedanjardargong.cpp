#include <bits/stdc++.h>
using namespace std;

pair<vector<int>, vector<vector<int>>> two_edge_components(int n, vector<vector<pair<int, int>>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), tec(n, -1);
    vector<vector<int>> tecs;
    stack<int> st;
    int count = 0;

    auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);

        for (auto [u, i] : adj_list[v])
            if (i != prev) {
                if (!order[u]) {
                    self(self, u, i);
                    low[v] = min(low[v], low[u]);
                } else low[v] = min(low[v], order[u]);
            }

        if (order[v] == low[v]) {
            tecs.emplace_back();
            int u;
            do {
                u = st.top();
                st.pop();
                tec[u] = tecs.size() - 1;
                tecs.back().emplace_back(u);
            } while (u != v);
        }
    };

    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return {tec, tecs};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 0; i < m; i++) {
        auto &[a, b] = edges[i];
        cin >> a >> b;
        a--;
        b--;

        adj_list[a].emplace_back(b, i);
        adj_list[b].emplace_back(a, i);
    }

    auto [tec, tecs] = two_edge_components(n, adj_list);
    if (tecs.size() == 1) {
        cout << 0;
        exit(0);
    }

    vector<int> degree(tecs.size(), 0);
    for (auto [a, b] : edges)
        if (tec[a] != tec[b]) {
            degree[tec[a]]++;
            degree[tec[b]]++;
        }

    int count = 0;
    for (int d : degree)
        if (!d) count += 2;
        else if (d == 1) count++;
    cout << (count + 1) / 2;
}
