#include <bits/stdc++.h>
using namespace std;

int play(int d, int n, int r, long total, vector<vector<vector<int>>> &bets, vector<vector<vector<int>>> &coins) {
    if (d >= total) return total;
    if (!d || !n || !r) return min(total, d * (1L << n));
    if (coins[d][n][r]) return coins[d][n][r];

    coins[d][n][r] = d;
    play(d - 1, n, r, total, bets, coins);

    int least = bets[d - 1][n][r], most = min(d, (int) (total - d));
    least = min(least, most);
    bets[d][n][r] = least;
    for (int i = least; i <= most; i++) {
        int max = min(play(d - i, n - 1, r - 1, total, bets, coins), play(d + i, n - 1, r, total, bets, coins));
        if (max < coins[d][n][r]) break;
        bets[d][n][r] = i;
        coins[d][n][r] = max;
    }

    return coins[d][n][r];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, g, n, k;
    cin >> d >> g >> n >> k;

    vector<vector<vector<int>>> coins(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0)));
    vector<vector<vector<int>>> bets(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0)));
    cout << play(d, n, n - k, d + g, bets, coins);
}
