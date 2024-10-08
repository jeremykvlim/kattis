#include <bits/stdc++.h>
using namespace std;

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
        auto ops = [&](auto &&self, int l, int r) -> int {
            if (l > r) return 0;

            if (dp[l][r] == -1) {
                dp[l][r] = self(self, l, r - 1) + 1;

                for (int m = l; m < r; m++)
                    if (s[m] == s[r]) dp[l][r] = min(dp[l][r], self(self, l, m) + self(self, m + 1, r - 1));
            }

            return dp[l][r];
        };
        cout << s.size() + 2 * ops(ops, 0, s.size() - 1) << "\n";
    }
}
