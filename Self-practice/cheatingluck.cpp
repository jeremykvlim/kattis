#include <bits/stdc++.h>
using namespace std;

int play(int d, int n, int r, long long total, vector<vector<vector<int>>> &bets, vector<vector<vector<int>>> &coins) {
    if (d >= total) return (int) total;
    if (!d || !n || !r) return (int) min(total, d * (1LL << n));
    if (coins[d][n][r]) return coins[d][n][r];

    coins[d][n][r] = d;
    play(d - 1, n, r, total, bets, coins);

    int bet = min(d, (int) (total - d)), least = min(bets[d - 1][n][r], bet);
    bets[d][n][r] = least;
    for (int b = least; b <= bet; b++) {
        auto most = min(play(d - b, n - 1, r - 1, total, bets, coins), play(d + b, n - 1, r, total, bets, coins));
        if (coins[d][n][r] > most) break;
        bets[d][n][r] = b;
        coins[d][n][r] = most;
    }

    return coins[d][n][r];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, g, n, k;
    cin >> d >> g >> n >> k;

    vector<vector<vector<int>>> coins(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0))), bets(d + g, vector<vector<int>>(n + 1, vector<int>(n + 1, 0)));

    cout << play(d, n, n - k, d + g, bets, coins);
}
