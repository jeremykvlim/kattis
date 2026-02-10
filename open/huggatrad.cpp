#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    vector<int> prev(n + 1, 0), order;
    prev[1] = -1;
    auto dfs = [&](auto &&self, int v = 1) -> void {
        order.emplace_back(v);
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
    };
    dfs(dfs);

    vector<int> dp1(n + 1, 0), dp2(n + 1, 0);
    reverse(order.begin(), order.end());
    for (int v : order) {
        int base = 0, d1 = 0, d2 = 0;
        for (int u : adj_list[v])
            if (prev[u] == v) {
                base += dp2[u];
                int delta = dp1[u] - dp2[u] + 1;
                if (d1 < delta) {
                    d2 = d1;
                    d1 = delta;
                } else d2 = max(d2, delta);
            }
        dp1[v] = base + d1;
        dp2[v] = base + d1 + d2;
    }
    cout << n - dp2[1];
}
