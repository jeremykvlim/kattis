#include <bits/stdc++.h>
using namespace std;

int bags(vector<vector<int>> &dp, int t, int n1, int n2, int s1, int s2) {
    if (dp[n1][n2] == -1) {
        dp[n1][n2] = n1 + n2;
        if (n1 || n2)
            for (int j = 0; j <= n2 && j * s2 <= t; j++) {
                int i = min((t - j * s2) / s1, n1);
                if (i | j) dp[n1][n2] = min(dp[n1][n2], bags(dp, t, n1 - i, n2 - j, s1, s2) + 1);
            }
    }

    return dp[n1][n2];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n1, n2, s1, s2;
    cin >> t >> n1 >> n2 >> s1 >> s2;

    vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, -1));
    cout << bags(dp, t, n1, n2, s1, s2);
}
