#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, s1, s2;
    cin >> s >> s1 >> s2;

    vector<vector<bool>> dp(s1.size() + 1, vector<bool>(s2.size() + 1, true));
    auto possible = [&](auto &&self, int i = 0, int j = 0, int k = 0) -> bool {
        if (k == s.size()) return i == s1.size() && j == s2.size();
        if (!dp[i][j]) return false;

        dp[i][j] = false;
        if (i < s1.size() && s[k] == s1[i]) return dp[i][j] = self(self, i + 1, j, k + 1);
        if (j < s2.size() && s[k] == s2[j]) return dp[i][j] = dp[i][j] || self(self, i, j + 1, k + 1);
        return dp[i][j];
    };
    cout << (possible(possible) ? "yes" : "no");
}
