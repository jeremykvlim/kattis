#include <bits/stdc++.h>
using namespace std;

pair<int, vector<int>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1, 0), component(n + 1, 0);
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
            } else if (stacked[u]) low[v] = min(low[v], order[u]);

        if (order[v] == low[v]) {
            sccs++;
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
            } while (u != v);
        }
    };

    for (int v = 1; v <= n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> f(n + 1), p(n + 1), m(n + 1), s(n + 1);
    for (int i = 1; i <= n; i++) cin >> f[i] >> p[i] >> m[i] >> s[i];

    vector<vector<int>> adj_list(n + 1);
    unordered_set<int> cheaper;
    for (int i = 1; i <= n; i++)
        if (m[f[i]] > p[i]) {
            cheaper.emplace(i);
            adj_list[i].emplace_back(f[i]);
        }

    auto [sccs, component] = tarjan(n, adj_list);

    vector<int> same(n + 1, 0), diff(n + 1, 0), sales(n + 1, INT_MIN);
    for (int i : cheaper)
        if (component[i] == component[f[i]]) same[f[i]] = max(same[f[i]], m[f[i]] - p[i]);
        else diff[f[i]] = max(diff[f[i]], m[f[i]] - p[i]);

    vector<bool> visited(n + 1, false);
    auto profit = 0LL;
    for (int i = 1; i <= n; i++) {
        profit += (long long) s[i] * max(same[i], diff[i]);

        if (i == f[i] || same[i] <= diff[i]) visited[component[i]] = true;
        else sales[component[i]] = max(sales[component[i]], diff[i] - same[i]);
    }

    for (int i = 1; i <= sccs; i++)
        if (!visited[i]) profit += sales[i];

    cout << profit;
}
