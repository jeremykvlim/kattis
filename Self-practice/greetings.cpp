#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<tuple<long long, long long, long long>> cards(n);
    vector<long long> waste((1 << n));
    vector<vector<long long>> dp(max(n, k), vector<long long>((1 << n), 1e16));

    for (auto &c : cards) cin >> get<0>(c) >> get<1>(c) >> get<2>(c);

    for (int i = 0; i < 1 << n; i++) {
        long long widest = 0, highest = 0;
        for (int j = 0; j < n; j++) 
            if (i & (1 << j)) {
                widest = max(widest, get<0>(cards[j]));
                highest = max(highest, get<1>(cards[j]));
            }

        waste[i] = 0;
        for (int j = 0; j < n; j++)
            if (i & (1 << j)) waste[i] += (widest * highest - get<0>(cards[j]) * get<1>(cards[j])) * get<2>(cards[j]);
    }
    
    dp[0][0] = 0;
    for (int i = 1; i <= k; i++)
        for (int j = 0; j < 1 << n; j++)
            for (int mask = j; mask > 0; mask = (mask - 1) & j)
                dp[i][j] = min(dp[i][j], dp[i - 1][j & ~mask] + waste[mask]);

    cout << dp[min(k, n)][(1 << n) - 1];
}
