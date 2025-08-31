#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, s1, s2;
    cin >> s >> s1 >> s2;

    int n1 = s1.size(), n2 = s2.size();
    if (s.size() != n1 + n2) {
        cout << "no";
        exit(0);
    }

    vector<bool> dp(n2 + 1, false);
    dp[0] = true;
    for (int j = 1; j <= n2; j++) dp[j] = dp[j - 1] && (s[j - 1] == s2[j - 1]);
    for (int i = 1; i <= n1; i++) {
        dp[0] = dp[0] && (s[i - 1] == s1[i - 1]);
        for (int j = 1; j <= n2; j++) dp[j] = (dp[j] && s[i + j - 1] == s1[i - 1]) || (dp[j - 1] && s[i + j - 1] == s2[j - 1]);
    }
    cout << (dp[n2] ? "yes" : "no");
}
