#include <bits/stdc++.h>
using namespace std;

tuple<int, vector<int>, vector<vector<int>>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1, 0), component(n + 1, 0);
    vector<vector<int>> members(1);
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
            members.emplace_back();
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
                members[sccs].emplace_back(u);
            } while (u != v);
        }
    };

    for (int v = 1; v <= n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component, members};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list_regular(n + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list_regular[a].emplace_back(b);
    }

    auto [sccs, component, members] = tarjan(n, adj_list_regular);

    vector<vector<int>> dag(sccs + 1);
    for (int v = 1; v <= n; v++)
        for (int u : adj_list_regular[v])
            if (component[v] != component[u]) dag[component[v]].emplace_back(component[u]);

    for (auto &neighbors : dag) {
        sort(neighbors.begin(), neighbors.end());
        neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
    }

    vector<int> order;
    vector<bool> visited(sccs + 1, false);
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : dag[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    for (int i = 1; i <= sccs; i++)
        if (!visited[i]) dfs(dfs, i);

    vector<int> dp(sccs + 1, 0), next(sccs + 1, -1);
    for (int v : order) {
        for (int u : dag[v])
            if (dp[v] < dp[u]) {
                dp[v] = dp[u];
                next[v] = u;
            }
        dp[v] += members[v].size();
    }

    int total = 0, comp = max_element(dp.begin() + 1, dp.end()) - dp.begin();
    vector<int> bases;
    for (int c = comp; c != -1; c = next[c]) {
        total += members[c].size();
        for (int v : members[c]) bases.emplace_back(v);
    }

    cout << total << "\n";
    for (int b : bases) cout << b << " ";
    cout << "\n" << members[comp][0];
}
