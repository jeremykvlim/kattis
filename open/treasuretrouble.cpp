#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int S, T, C;
    cin >> S >> T >> C;

    vector<int> s(T);
    for (int &si : s) cin >> si;

    vector<int> count(T, 0);
    for (int _ = 0; _ < C; _++) {
        int k;
        cin >> k;

        vector<bool> seen(T, false);
        while (k--) {
            int t;
            cin >> t;

            if (!seen[t - 1]) {
                seen[t - 1] = true;
                count[t - 1]++;
            }
        }
    }

    if (!C) fill(count.begin(), count.end(), 1);
    else transform(count.begin(), count.end(), count.begin(), [&](int c) { return C - c; });

    vector<vector<bool>> selected(T + 1, vector<bool>(S + 1, false));
    vector<vector<int>> dp(T + 1, vector<int>(S + 1, -1e9));
    dp[0][0] = 0;
    for (int i = 0; i < T; i++)
        for (int j = 0; j <= S; j++)
            if (dp[i][j] != -1e9) {
                if (dp[i + 1][j] < dp[i][j]) {
                    dp[i + 1][j] = dp[i][j];
                    selected[i + 1][j] = false;
                }

                int k = j + s[i];
                if (k <= S && dp[i + 1][k] < dp[i][j] + count[i]) {
                    dp[i + 1][k] = dp[i][j] + count[i];
                    selected[i + 1][k] = true;
                }
            }

    for (int i = T, j = max_element(dp[T].begin(), dp[T].end()) - dp[T].begin(); i; i--)
        if (selected[i][j]) {
            cout << i << " ";
            j -= s[i - 1];
        }
}
