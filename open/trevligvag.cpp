#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int w_max = 0;
    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        w_max = max(w_max, w);
        adj_list[u].emplace_back(v, w);
    }

    vector<double> dp(n + 1);
    double l = 0, r = w_max, mid;
    while (l + 1e-9 < r && l + l * 1e-9 < r) {
        mid = l + (r - l) / 2;

        fill(dp.begin(), dp.end(), -1e20);
        dp[1] = 0;

        for (int v = 1; v <= n; v++)
            if (dp[v] != -1e20)
                for (auto [u, w] : adj_list[v]) dp[u] = max(dp[u], dp[v] + w - mid);
        if (dp[n] >= 0) l = mid;
        else r = mid;
    }
    cout << fixed << setprecision(6) << l;
}
