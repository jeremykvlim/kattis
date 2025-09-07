#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> gdp(n), population(n);
    for (int &c : gdp) cin >> c;
    for (int &k : population) cin >> k;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<double> cost(n), dp(n);
    double l = 0, r = 1e6, m;
    while (l + 1e-7 < r && l + l * 1e-7 < r) {
        m = l + (r - l) / 2;

        auto valid = [&]() -> bool {
            auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
                dp[v] = cost[v];
                for (int u : adj_list[v])
                    if (u != prev) {
                        self(self, u, v);
                        dp[v] += max(0., dp[u]);
                    }
            };

            for (int i = 0; i < cost.size(); i++) cost[i] = gdp[i] - m * population[i];
            dfs(dfs);

            return dp[0] < 0;
        };

        if (valid()) r = m;
        else l = m;
    }
    cout << fixed << setprecision(6) << r;
}
