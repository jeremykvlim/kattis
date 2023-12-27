#include <bits/stdc++.h>
using namespace std;

int shortest(int k, int d, vector<int> &days, vector<vector<int>> &dp) {
    if (!k) return d;
    if (dp[k][d] >= 0) return dp[k][d];
    dp[k][d] = INT_MAX;
    
    int prev = -1;
    for (int i = 0; i < days.size(); i++) {
        if (!(k & (1 << i)) || days[i] == prev) continue;
        prev = days[i];
        dp[k][d] = min(dp[k][d], d + 1 + shortest(k - (1 << i), max(0, d + 1 - days[i]) + days[i] - (d + 1), days, dp));
    }
    return dp[k][d];
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
