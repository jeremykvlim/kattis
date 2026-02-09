#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int p_max = 0;
    auto sum = 0LL;
    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;

        p_max = max(p_max, pi);
        sum += pi;
    }

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<int> prev(n, -1), order;
    auto dfs = [&](auto &&self, int v = 0) -> void {
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
        order.emplace_back(v);
    };
    dfs(dfs);

    vector<long long> dp(n);
    long long l = p_max - 1, r = sum + 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        auto valid = [&](long long m) -> bool {
            fill(dp.begin(), dp.end(), 0);
            for (int v : order) {
                auto s = 0LL;
                for (int u : adj_list[v])
                    if (prev[u] == v) s += dp[u];

                s = max(s, (long long) p[v]);
                if (v && s > 2 * m - p[prev[v]]) return false;
                dp[v] = s;
            }
            return dp[0] <= 2 * m;
        };

        if (valid(m)) r = m;
        else l = m;
    }
    cout << r;
}
