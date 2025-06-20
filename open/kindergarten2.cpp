#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> j(n + 1), l(n + 1);
    for (int i = 1; i <= n; i++) cin >> j[i] >> l[i];

    int t = 1;
    vector<int> visited(n + 1, 0);
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    auto dfs = [&](auto &&self, int v, int u, int prev = -1) -> bool {
        if (stacked[v]) {
            if (visited[v] <= visited[prev]) {
                st.emplace(v);
                return true;
            }
            return false;
        }

        st.emplace(v);
        stacked[v] = true;
        visited[v] = t++;

        if ((j[v] != u && self(self, j[v], u, v)) || self(self, l[v], j[v], prev)) return true;

        st.pop();
        stacked[v] = false;
        return false;
    };

    int k = 1;
    do {
        if (!dfs(dfs, k, j[k])) {
            k = j[k];
            continue;
        }

        vector<int> degree(n + 1, 0);
        vector<bool> seen(n + 1, false);
        vector<vector<int>> adj_list(n + 1);
        while (st.size() > 1) {
            int v = st.top();
            st.pop();

            seen[st.top()] = v == l[st.top()];
        }

        for (int v = 1; v <= n; v++)
            if (seen[v]) {
                adj_list[v].emplace_back(l[v]);
                adj_list[l[v]].emplace_back(j[v]);
                degree[l[v]]++;
                degree[j[v]]++;
            } else {
                adj_list[j[v]].emplace_back(v);
                degree[v]++;
            }

        vector<int> order;
        for (int v = 1; v <= n; v++)
            if (!degree[v]) order.emplace_back(v);

        for (int i = 0; i < order.size(); i++) {
            int v = order[i];
            for (int u : adj_list[v])
                if (!--degree[u]) order.emplace_back(u);
        }
        for (int v : order) cout << v << " ";
        exit(0);
    } while (k != 1);
    cout << "impossible";
}
