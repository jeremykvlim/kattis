#include <bits/stdc++.h>
using namespace std;

int buy(int cokes, int ones, int fives, vector<vector<vector<int>>> &dp) {
    if (dp[cokes][ones][fives] != -1) return dp[cokes][ones][fives];
    if (!cokes) return dp[cokes][ones][fives] = 0;

    int coins = INT_MAX;
    if (ones >= 8) coins = min(coins, 8 + buy(cokes - 1, ones - 8, fives, dp));
    if (ones >= 3 && fives >= 1) coins = min(coins, 4 + buy(cokes - 1, ones - 3, fives - 1, dp));
    if (fives >= 2) coins = min(coins, 2 + buy(cokes - 1, ones + 2, fives - 2, dp));
    return dp[cokes][ones][fives] = coins;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
  
    vector<vector<vector<int>>> dp;
    while (t--) {
        int c, n1, n5, n10;
        cin >> c >> n1 >> n5 >> n10;

        int coins = INT_MAX, tens = min(c, n10);

        dp.resize(c + 1);
        for (auto &i : dp) {
            i.resize(n1 + 2 * tens + n5 + tens + 1);
            for (auto &j : i) j.resize(n5 + tens + 1, -1);
        }

        for (int i = 0; i <= tens && n1 >= 3 * i; i++) coins = min(coins, tens - i + 4 * i + buy(c - tens, n1 - 3 * i + 2 * (tens - i), n5 + i, dp));
        cout << coins << "\n";
    }
}
