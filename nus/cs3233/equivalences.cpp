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

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        if (!m) {
            cout << n << "\n";
            continue;
        }

        vector<vector<int>> adj_list(n + 1);
        while (m--) {
            int s1, s2;
            cin >> s1 >> s2;

            adj_list[s1].emplace_back(s2);
        }

        auto [sccs, component] = tarjan(n, adj_list);

        if (sccs == 1) {
            cout << "0\n";
            continue;
        }

        vector<int> in(sccs + 1, 0), out(sccs + 1, 0);
        for (int v = 1; v <= n; v++)
            for (int u : adj_list[v])
                if (component[u] != component[v]) {
                    in[component[u]]++;
                    out[component[v]]++;
                }

        cout << max(count_if(in.begin() + 1, in.end(), [](int d) { return !d; }), count_if(out.begin() + 1, out.end(), [](int d) { return !d; })) << "\n";
    }
}
