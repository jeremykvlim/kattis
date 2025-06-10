#include <bits/stdc++.h>
using namespace std;

vector<int> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1, 0), scc_roots;
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v = 1) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
            } else if (stacked[u]) low[v] = min(low[v], order[u]);

        int size = 0;
        if (order[v] == low[v]) {
            sccs++;
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                size++;
            } while (u != v);

            if (size > 1) scc_roots.emplace_back(v);
        }
    };
    dfs(dfs, 1);

    return scc_roots;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
    }
    auto roots = tarjan(n, adj_list);
    if (!roots.empty()) {
        queue<int> q;
        vector<bool> visited(n + 1, false);
        for (int v : roots) {
            q.emplace(v);
            visited[v] = true;
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (!visited[u]) {
                    if (u == 2) {
                        cout << "inf";
                        exit(0);
                    }
                    visited[u] = true;
                    q.emplace(u);
                }
        }
    }

    vector<int> order;
    vector<bool> visited(n + 1, false);
    auto dfs = [&](auto &&self, int v = 1) -> void {
        visited[v] = true;
        for (int u : adj_list[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    dfs(dfs);
    reverse(order.begin(), order.end());

    bool zero = false;
    vector<int> dp(n + 1, 0);
    dp[1] = 1;
    for (int v : order)
        for (int u : adj_list[v]) {
            dp[u] += dp[v];
            if (dp[u] >= 1e9) {
                zero = true;
                dp[u] %= (int) 1e9;
            }
        }

    if (!zero) cout << dp[2];
    else cout << setfill('0') << setw(9) << dp[2];
}
