#include <bits/stdc++.h>
using namespace std;

int play(int d, int n, int r, long long total, vector<vector<vector<int>>> &bets, vector<vector<vector<int>>> &dp) {
    if (d >= total) return (int) total;
    if (!d || !n || !r) return (int) min(total, d * (1LL << n));
    if (dp[d][n][r]) return dp[d][n][r];

    dp[d][n][r] = d;
    play(d - 1, n, r, total, bets, dp);

    int bet = min(d, (int) (total - d)), least = min(bets[d - 1][n][r], bet);
    bets[d][n][r] = least;
    for (int b = least; b <= bet; b++) {
        int most = min(play(d - b, n - 1, r - 1, total, bets, dp), play(d + b, n - 1, r, total, bets, dp));
        if (dp[d][n][r] > most) break;
        bets[d][n][r] = b;
        dp[d][n][r] = most;
    }

    return dp[d][n][r];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, g, n, k;
    cin >> d >> g >> n >> k;

    vector<vector<vector<int>>> bets(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0))), dp(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0)));
    cout << play(d, n, n - k, d + g, bets, dp);
}
