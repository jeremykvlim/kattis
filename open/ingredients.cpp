#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, n;
    cin >> b >> n;

    unordered_map<string, int> compress;
    vector<vector<array<int, 3>>> adj_list;
    while (n--) {
        string derived, base, _;
        int p1, p2;
        cin >> derived >> base >> _ >> p1 >> p2;

        if (!compress.count(derived)) {
            compress[derived] = compress.size();
            adj_list.emplace_back();
        }
        if (!compress.count(base)) {
            compress[base] = compress.size();
            adj_list.emplace_back();
        }
        adj_list[compress[derived]].push_back({compress[base], p1, p2});
    }

    vector<int> price(adj_list.size(), -1), prestige(adj_list.size(), -1);
    auto dfs = [&](auto &&self, int v) {
        if (price[v] != -1) return;
        if (adj_list[v].empty()) {
            price[v] = prestige[v] = 0;
            return;
        }
        price[v] = 1e9;
        prestige[v] = 0;
        for (auto &[u, p1, p2] : adj_list[v]) {
            self(self, u);
            if (price[v] > price[u] + p1 || (price[v] == price[u] + p1 && prestige[v] < prestige[u] + p2)) {
                price[v] = price[u] + p1;
                prestige[v] = prestige[u] + p2;
            }
        }
    };
    for (int v = 0; v < adj_list.size(); v++) dfs(dfs, v);

    vector<int> dp(b + 1, 0);
    for (int v = 0; v < adj_list.size(); v++)
        for (int p = b; p >= price[v]; p--) dp[p] = max(dp[p], dp[p - price[v]] + prestige[v]);

    int p = b;
    for (; p && dp[p - 1] == dp[b]; p--);
    cout << dp[b] << "\n" << p << "\n";
}
