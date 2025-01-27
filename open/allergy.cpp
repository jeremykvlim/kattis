#include <bits/stdc++.h>
using namespace std;

int shortest(int mask, int d, vector<int> &days, vector<vector<int>> &dp) {
    if (!mask) return d;
    if (dp[mask][d] >= 0) return dp[mask][d];
    dp[mask][d] = INT_MAX;

    int prev = -1;
    for (int i = 0; i < days.size(); i++) {
        if (!((mask >> i) & 1) || days[i] == prev) continue;
        prev = days[i];
        dp[mask][d] = min(dp[mask][d], d + 1 + shortest(mask - (1 << i), max(0, d + 1 - days[i]) + days[i] - (d + 1), days, dp));
    }
    return dp[mask][d];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<int> days(k);
    for (int &d : days) cin >> d;
    sort(days.begin(), days.end());

    vector<vector<int>> dp(1 << k, vector<int>(days.back(), -1));
    cout << shortest((1 << k) - 1, 0, days, dp);
}
