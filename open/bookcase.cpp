#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int t_max = -1;
    vector<pair<int, int>> books(n);
    for (auto &[h, t] : books) {
        cin >> h >> t;

        t_max = max(t_max, t);
    }
    sort(books.rbegin(), books.rend());

    vector<int> pref(n, 0);
    for (int i = 1; i < n; i++) pref[i] = pref[i - 1] + books[i].second;

    vector<vector<int>> dp(t_max * n, vector<int>(t_max * n, -1));
    dp[0][0] = 0;
    int area = INT_MAX;
    for (int i = 1; i < n; i++)
        for (int j = min(pref[i], (pref[i] + t_max) / 2 + 1); ~j; j--)
            for (int k = min(pref[i] - j, j + t_max); ~k; k--) {
                auto [h, t] = books[i];

                if (j >= t && ~dp[j - t][k])
                    if (!~dp[j][k] || dp[j][k] > dp[j - t][k] + (j == t ? h : 0))
                        dp[j][k] = dp[j - t][k] + (j == t ? h : 0);

                if (k >= t && ~dp[j][k - t])
                    if (!~dp[j][k] || dp[j][k] > dp[j][k - t] + (k == t ? h : 0))
                        dp[j][k] = dp[j][k - t] + (k == t ? h : 0);

                if (i == n - 1 && j && k && ~dp[j][k])
                    area = min(area, (books[0].first + dp[j][k]) * max(pref[n - 1] + books[0].second - j - k, max(j, k)));
            }

    cout << area;
}
