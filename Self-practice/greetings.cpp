#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<tuple<int, int, int>> cards(n);
    for (auto &c : cards) cin >> get<0>(c) >> get<1>(c) >> get<2>(c);

    vector<long long> waste(1 << n);
    vector<vector<long long>> dp(max(n, k) + 1, vector<long long>(1 << n, 1e13));

    for (int i = 0; i < 1 << n; i++) {
        int w = 0, h = 0;
        for (int j = 0; j < n; j++)
            if (i & (1 << j)) {
                w = max(w, get<0>(cards[j]));
                h = max(h, get<1>(cards[j]));
            }

        waste[i] = 0;
        for (int j = 0; j < n; j++)
            if (i & (1 << j)) waste[i] += ((long long) w * h - (long long) get<0>(cards[j]) * get<1>(cards[j])) * get<2>(cards[j]);
    }

    dp[0][0] = 0;
    for (int i = 1; i <= k; i++)
        for (int j = 0; j < 1 << n; j++)
            for (int mask = j; mask > 0; mask = (mask - 1) & j)
                dp[i][j] = min(dp[i][j], dp[i - 1][j & ~mask] + waste[mask]);


    cout << dp[min(n, k)][(1 << n) - 1];
}
