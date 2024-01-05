#include <bits/stdc++.h>
using namespace std;

int play(int d, int n, int r, long total, vector<vector<vector<int>>> &bets, vector<vector<vector<int>>> &dp) {
    if (d >= total) return total;
    if (!d || !n || !r) return min(total, d * (1L << n));
    if (dp[d][n][r]) return dp[d][n][r];

    dp[d][n][r] = d;
    play(d - 1, n, r, total, bets, dp);

    int least = bets[d - 1][n][r], most = min(d, (int) (total - d));
    least = min(least, most);
    bets[d][n][r] = least;
    for (int i = least; i <= most; i++) {
        int max = min(play(d - i, n - 1, r - 1, total, bets, dp), play(d + i, n - 1, r, total, bets, dp));
        if (max < dp[d][n][r]) break;
        bets[d][n][r] = i;
        dp[d][n][r] = max;
    }

    return dp[d][n][r];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, g, n, k;
    cin >> d >> g >> n >> k;

    vector<vector<vector<int>>> dp(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0))), bets(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0)));
    
    cout << play(d, n, n - k, d + g, bets, dp);
}
