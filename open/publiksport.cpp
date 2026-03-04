#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> adj_list(n + 1);
    vector<int> degree(n + 1, 0);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
        degree[a]++;
        degree[b]++;
    }

    if (k == 1) {
        cout << *max_element(degree.begin(), degree.end());
        exit(0);
    }

    vector<vector<int>> children(n + 1);
    vector<int> depth(n + 1, 0), pref(n + 1, 0), heavy(n + 1, 0);
    pref[0] = degree[0];
    auto hld = [&](auto &&self, int v = 1, int prev = -1) -> int {
        int subtree_size = 1, largest = 0;
        for (int u : adj_list[v])
            if (u != prev) {
                children[v].emplace_back(u);
                depth[u] = depth[v] + 1;
                pref[u] = pref[v] + degree[u];
                int size = self(self, u, v);
                subtree_size += size;
                if (largest < size) {
                    largest = size;
                    heavy[v] = u;
                }
            }
        return subtree_size;
    };
    hld(hld);

    vector<int> tour, in(n + 1), out(n + 1);
    int count = 0;
    auto dfs1 = [&](auto &&self, int v = 1) -> void {
        tour.emplace_back(v);
        in[v] = count++;
        for (int u : children[v]) self(self, u);
        out[v] = count;
    };
    dfs1(dfs1);

    int sum = -1e9;
    queue<int> q;
    vector<int> dp(*max_element(depth.begin(), depth.end()) + k, -1e9);
    auto dfs2 = [&](auto &&self, int v = 1, bool undo = false) -> void {
        for (int u : children[v])
            if (u != heavy[v]) self(self, u, true);
        if (heavy[v]) self(self, heavy[v], false);

        dp[depth[v]] = max(dp[depth[v]], pref[v]);

        int d = depth[v] + k - 1;
        if (dp[d] != -1e9) sum = max(sum, dp[d] + degree[v] - pref[v]);

        for (int u : children[v])
            if (u != heavy[v]) {
                int base = 2 * depth[v] + k - 1;
                for (int i = in[u]; i < out[u]; i++) {
                    int t = tour[i];
                    if (base >= depth[t] && dp[base - depth[t]] != -1e9) sum = max(sum, dp[base - depth[t]] + degree[v] - 2 * pref[v] + pref[t]);
                    q.emplace(t);
                }

                while (!q.empty()) {
                    int t = q.front();
                    q.pop();

                    dp[depth[t]] = max(dp[depth[t]], pref[t]);
                }
            }

        if (undo)
            for (int i = in[v]; i < out[v]; i++) dp[depth[tour[i]]] = -1e9;
    };
    dfs2(dfs2);

    if (sum == -1e9) cout << "-";
    else cout << sum - 2 * (k - 1);
}
