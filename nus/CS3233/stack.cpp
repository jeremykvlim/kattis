#include <bits/stdc++.h>
using namespace std;

int ops(vector<vector<int>> &dp, string &s, int i, int j) {
    if (i > j) return 0;
    if (dp[i][j] == -1) {
        dp[i][j] = ops(dp, s, i, j - 1) + 1;

        for (int k = i; k < j; k++)
            if (s[k] == s[j])
                dp[i][j] = min(dp[i][j], ops(dp, s, i, k) + ops(dp, s, k + 1, j - 1));
    }

    return dp[i][j];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    cin.ignore();

    while (t--) {
        string s;
        getline(cin, s);

        vector<vector<int>> dp(s.size(), vector<int>(s.size(), -1));
        cout << s.size() + 2 * ops(dp, s, 0, s.size() - 1) << "\n";
    }
}
