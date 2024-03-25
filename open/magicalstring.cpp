#include <bits/stdc++.h>
using namespace std;

int convert(string &s, vector<vector<int>> &memo, int i, int j) {
    if (memo[i][j] != -1) return memo[i][j];

    vector<int> freq(27, 0);
    for (int k = i; k < i + 3 + j; k++) freq[s[k] - 'a' + 1]++;

    int unique = 0, l = 0;
    for (int k = 1; k <= 26; k++)
        if (freq[k]) {
            unique++;
            if (!l || freq[l] > freq[k]) l = k;
        }

    if (unique == 2 && freq[l] == 1)
        for (int k = 1; k <= 26; k++)
            if (freq[k] && l != k) return memo[i][j] = k;

    return memo[i][j] = 0;
}

int turn(string &s, int n, bool consecutive, int i, int k, vector<vector<vector<int>>> &dp, vector<vector<int>> &memo) {
    if (k < 0) return INT_MIN;
    if (i >= n - 2) return 0;
    if (dp[consecutive][i][k] != -1) return dp[consecutive][i][k];

    int turned = INT_MIN;
    for (int j = 3; j <= 5; j++) {
        if (j + i - 1 >= n) break;
        int c = convert(s, memo, i, j - 3);
        if (c && (!consecutive || (c != s[i - 1] - 'a' + 1 || convert(s, memo, i - 3, 0) != s[i] - 'a' + 1)))
            turned = max(turned, turn(s, n, true, i + j, k - 1, dp, memo) + j);
    }

    return dp[consecutive][i][k] = max(turned, turn(s, n, false, i + 1, k, dp, memo));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s;
        int k;
        cin >> s >> k;

        int n = s.size();
        vector<vector<vector<int>>> dp(2, vector<vector<int>>(n, vector<int>(k + 1, -1)));
        vector<vector<int>> memo(n, vector<int>(3, -1));

        cout << turn(s, n, false, 0, k, dp, memo) << "\n";
    }
}
