#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();
    vector<vector<long long>> dp(n + 1, vector<long long>(n, 0));
    for (int i = 1; i <= n; i++) dp[i][i - 1] = 1;

    vector<char> open{'(', '[', '{', '<'}, close{')', ']', '}', '>'};
    for (int l = n - 1; ~l; l--)
        for (int r = l; r < n; r++)
            if (!((r - l + 1) & 1))
                for (int k = 0; k < 4; k++)
                    if (s[l] == open[k] || s[l] == '?')
                        for (int i = l + 1; i <= r; i++)
                            if (s[i] == close[k] || s[i] == '?') dp[l][r] += dp[l + 1][i - 1] * dp[i + 1][r];

    cout << dp[0][n - 1];
}
