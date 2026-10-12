#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> k(n);
    for (int &ki : k) cin >> ki;

    vector<int> dp(n + 1, 0);
    vector<vector<int>> pos(n);
    for (int i = 0; i < n; i++) {
        dp[i + 1] = dp[i] + 1;
        pos[i].emplace_back(i);
        for (int j = i, p = k[i]; j && k[j - 1] <= p && p < k[j - 1] + pos[j - 1].size(); p++) {
            j = pos[j - 1][p - k[j - 1]];
            dp[i + 1] = min(dp[i + 1], dp[j] + 1);
            pos[i].emplace_back(j);
        }
    }
    cout << dp[n];
}