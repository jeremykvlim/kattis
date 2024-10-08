#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();
    vector<vector<vector<bool>>> dp(n + 1, vector<vector<bool>>(n + 1, vector<bool>(n + 1, false)));
    vector<vector<vector<int>>> visited(n + 1, vector<vector<int>>(n + 1, vector<int>(n + 1, -1)));

    for (int i = 1, t = 1; i <= n; i++)
        if (!(n % i)) {
            string p;
            for (int j = 0; j <= n - i; j++, t++) {
                auto sub = s.substr(j, i);

                auto valid = [&](auto &&self, string sub, int l, int r, int k = 0) -> bool {
                    if (l > r) return !k;
                    if (r - l + 1 - i + k < 0 || (r - l + 1 - i + k) % i || s[l] != sub[k]) return false;
                    if (visited[l][r][k] == t) return dp[l][r][k];

                    visited[l][r][k] = t;
                    if (k == i - 1) return dp[l][r][k] = self(self, sub, l + 1, r);
                    for (int j = l + 1; j <= r; j++)
                        if (s[j] == sub[k + 1])
                            if (self(self, sub, l + 1, j - 1) && self(self, sub, j, r, k + 1)) return dp[l][r][k] = true;

                    return dp[l][r][k] = false;
                };
                if (valid(valid, sub, 0, n - 1)) p = !p[0] ? sub : min(p, sub);
            }

            if (p[0]) {
                cout << p;
                exit(0);
            }
        }
}
