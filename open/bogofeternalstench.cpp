#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    vector<array<int, 3>> edges(m);
    for (auto &[u, v, s] : edges) {
        cin >> u >> v >> s;

        adj_list[u].emplace_back(v);
    }

    vector<long long> dp1(n + 1, 1e18), dp2(n + 1, 1e18);
    dp1[1] = 0;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list[v])
            if (dp1[u]) {
                q.emplace(u);
                dp1[u] = 0;
            }
    }

    for (int i = 0; i < n; i++) {
        vector<long long> temp(n + 1, 1e18);
        temp[1] = 0;
        for (auto [u, v, s] : edges) {
            temp[v] = min(temp[v], max(0LL, dp1[u] + s));
            dp2[v] = min(dp2[v], max(0LL, dp2[u] + s));
        }
        dp1 = temp;
    }

    for (auto [u, v, s] : edges)
        if (dp2[v] > max(0LL, dp2[u] + s)) dp2[v] = min(dp2[v], dp1[v]);

    dp2[1] = 0;
    for (int i = 0; i < n; i++)
        for (auto [u, v, s] : edges) dp2[v] = min(dp2[v], max(0LL, dp2[u] + s));
    cout << dp2[n];
}
