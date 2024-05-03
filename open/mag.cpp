#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    vector<int> dp1(n), dp2(n);
    auto dfs = [&](auto &&self, int v, int prev, bool first) -> void {
        auto update = [&](int i, int j) -> void {
            dp2[i] = max(dp2[i], j);
            if (dp2[i] > dp1[i]) swap(dp1[i], dp2[i]);
        };
        
        for (int u : adj_list[v])
            if (u != prev) {
                if (first) {
                    self(self, u, v, first);
                    if (!~-x[u]) update(v, dp1[u] + 1);
                } else {
                    if (!~-x[v]) update(u, (dp1[u] + 1 == dp1[v] ? dp2[v] : dp1[v]) + 1);
                    self(self, u, v, first);
                }
            }
    };
    dfs(dfs, 0, -1, true);
    dfs(dfs, 0, -1, false);

    auto p = 1LL, q = 0LL;
    for (int i = 0; i < n; i++) {
        int a = x[i], b = dp1[i] + dp2[i] + 1;
        if ((long long) a * q < (long long) b * p) p = a, q = b;
    }
    auto g = __gcd(p, q);

    cout << p / g << "/" << q / g;
}
