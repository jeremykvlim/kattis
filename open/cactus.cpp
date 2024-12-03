#include <bits/stdc++.h>
using namespace std;

int tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1, 0);
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        int simple = 0;
        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
                if (low[u] != order[v]) simple++;
            } else if (stacked[u]) {
                low[v] = min(low[v], order[u]);
                simple++;
            }

        if (simple > 1) {
            cout << "NO";
            exit(0);
        }

        if (order[v] == low[v]) {
            sccs++;
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
            } while (u != v);
        }
    };

    for (int v = 1; v <= n; v++)
        if (!order[v]) dfs(dfs, v);

    return sccs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u + 1].emplace_back(v + 1);
    }
    
    cout << (tarjan(n, adj_list) == 1 ? "YES" : "NO");
}
