#include <bits/stdc++.h>
using namespace std;

bool verify(string &s, string &s1, string &s2, vector<vector<bool>> &dp, int i = 0, int j = 0, int k = 0) {
    if (k == s.size()) return i == s1.size() && j == s2.size();
    if (!dp[i][j]) return dp[i][j];
    dp[i][j] = false;
    if (s[k] != s1[i] && s[k] != s2[j]) return dp[i][j];
    if (s[k] == s1[i]) dp[i][j] = verify(s, s1, s2, dp, i + 1, j, k + 1);
    if (s[k] == s2[j]) dp[i][j] = dp[i][j] || verify(s, s1, s2, dp, i, j + 1, k + 1);
    return dp[i][j];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, s1, s2;
    cin >> s >> s1 >> s2;
    
    vector<vector<bool>> dp(s1.size() + 1, vector<bool>(s2.size() + 1, true));
    cout << (verify(s, s1, s2, dp) ? "yes" : "no");
}
