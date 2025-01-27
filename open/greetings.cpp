#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<array<int, 3>> cards(n);
    for (auto &[w, h, q] : cards) cin >> w >> h >> q;

    vector<long long> waste(1 << n);
    for (int mask = 0; mask < 1 << n; mask++) {
        int w = 0, h = 0;
        for (int i = 0; i < n; i++)
            if ((mask >> i) & 1) {
                w = max(w, cards[i][0]);
                h = max(h, cards[i][1]);
            }

        waste[mask] = 0;
        for (int i = 0; i < n; i++)
            if ((mask >> i) & 1) waste[mask] += ((long long) w * h - (long long) cards[i][0] * cards[i][1]) * cards[i][2];
    }

    vector<vector<long long>> dp(max(n, k) + 1, vector<long long>(1 << n, 1e13));
    dp[0][0] = 0;
    for (int i = 1; i <= k; i++)
        for (int j = 0; j < 1 << n; j++)
            for (int mask = j; mask; --mask &= j) dp[i][j] = min(dp[i][j], dp[i - 1][j & ~mask] + waste[mask]);

    cout << dp[min(n, k)].back();
}
