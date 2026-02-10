#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<unsigned> adj_masks(n, 0);
    vector<int> degree(n, 0);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        adj_masks[a] |= 1 << b;
        adj_masks[b] |= 1 << a;
        degree[a]++;
        degree[b]++;
    }

    vector<int> dp(1 << n, -1e9);
    dp.back() = 0;
    for (int mask = (1 << n) - 1; ~mask; mask--)
        for (int v = 0; v < n; v++)
            if (!((mask >> v) & 1)) dp[mask] = max(dp[mask], 2 * popcount(adj_masks[v] & mask) - degree[v] - dp[mask | (1 << v)]);

    if (dp[0] > 0) cout << "player 1";
    else if (dp[0] < 0) cout << "player 2";
    else cout << "tie";
}
