#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<char> letters(k);
    vector<int> indices(26, -1);
    for (int i = 0; i < k; i++) {
        cin >> letters[i];

        indices[letters[i] - 'a'] = i;
    }

    vector<vector<pair<int, int>>> table(k, vector<pair<int, int>>(k));
    for (auto &row : table)
        for (auto &[time, result] : row) {
            char _, c;
            cin >> time >> _ >> c;

            result = indices[c - 'a'];
        }

    int n;
    cin >> n;

    while (n--) {
        string s;
        cin >> s;
        int m = s.size();

        vector<vector<vector<int>>> dp(m + 1, vector<vector<int>>(m + 1, vector<int>(k, 1e9)));
        for (int i = 0; i < m; i++) dp[i][i + 1][indices[s[i] - 'a']] = 0;

        for (int len = 2; len <= m; len++)
            for (int l = 0, r = len; r <= m; l++, r++)
                for (int mid = l + 1; mid < r; mid++)
                    for (int i = 0; i < k; i++)
                        if (dp[l][mid][i] != 1e9)
                            for (int j = 0; j < k; j++) {
                                auto [time, result] = table[i][j];
                                dp[l][r][result] = min(dp[l][r][result], dp[l][mid][i] + dp[mid][r][j] + time);
                            }

        int result = min_element(dp[0][m].begin(), dp[0][m].end()) - dp[0][m].begin();
        cout << dp[0][m][result] << "-" << letters[result] << "\n";
    }
}
