#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int p, q;
        cin >> p >> q;

        adj_list[p].emplace_back(q);
        adj_list[q].emplace_back(p);
    }

    vector<int> prev(n, -1), order;
    auto dfs = [&](auto &&self, int v = 0) -> void {
        order.emplace_back(v);
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
    };
    dfs(dfs);

    reverse(order.begin(), order.end());
    vector<array<int, 7>> dp1(n);
    for (int i : order) {
        for (int d = 0; d < 5; d++) dp1[i][d] = a[i] / (b[i] + d * d);

        for (int j : adj_list[i])
            if (prev[j] == i)
                for (int d = 0; d < 5; d++) dp1[i][d] += dp1[j][d + 1];
    }

    int k = 0, sum = -1;
    reverse(order.begin(), order.end());
    vector<array<int, 7>> dp2(n);
    for (int i : order) {
        int s = dp1[i][0] + dp2[i][0];
        if (sum < s) {
            sum = s;
            k = i;
        }

        for (int j : adj_list[i])
            if (prev[j] == i)
                for (int d = 0; d < 5; d++) dp2[j][d] = dp2[i][d + 1] + dp1[i][d + 1] - dp1[j][d + 2];
    }
    cout << k;
}
