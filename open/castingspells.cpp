#include <bits/stdc++.h>
using namespace std;

vector<int> manacher(string s) {
    int n = s.size();
    if (n <= 1) return {};

    vector<int> dp(2 * n - 1, 0);
    for (int k = 0, l = -1, r = -1; k < 2 * n - 1; k++) {
        int i = (k + 1) >> 1, j = k >> 1, p = i >= r ? 0 : min(r - i, dp[2 * (l + r) - k]);
        while (j + p + 1 < n && i - p - 1 >= 0 && s[j + p + 1] == s[i - p - 1]) p++;

        if (r < j + p) {
            r = j + p;
            l = i - p;
        }

        dp[k] = p;
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    auto dp = manacher(s);
    auto k = 0LL;
    for (int i = 1; i < 2 * s.size() - 1; i += 2) {
        int r = dp[i] * 2 - dp[i] * 2 % 4;
        if (r > k)
            for (int j = r / 2; j > k / 2; j--)
                if (dp[i - j] * 2 >= j) {
                    k = j * 2 - (j * 2) % 4;
                    break;
                }
    }
    cout << k;
}
