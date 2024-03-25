#include <bits/stdc++.h>
using namespace std;

int find(int l, int r, int count, vector<vector<vector<int>>> &dp, vector<int> &colors, int k) {
    if (l > r) return 0;
    if (l == r) return k - 1 - count;
    if (dp[l][r][count] != -1) return dp[l][r][count];

    dp[l][r][count] = INT_MAX;
    if (count < k - 1) dp[l][r][count] = find(l, r, count + 1, dp, colors, k) + 1;
    if (count == k - 1) dp[l][r][count] = find(l + 1, r, 0, dp, colors, k);

    for (int i = l + 1; i <= r; i++)
        if (colors[i] == colors[l]) dp[l][r][count] = min(find(l + 1, i - 1, 0, dp, colors, k) + find(i, r, min(k - 1, count + 1), dp, colors, k), dp[l][r][count]);

    return dp[l][r][count];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> colors(n);
    for (int &c : colors) cin >> c;

    vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(k, -1)));
    cout << find(0, n - 1, 0, dp, colors, k);
}
