#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> cells{0};
    while (n--) {
        int e;
        cin >> e;

        if (e != cells.back()) cells.emplace_back(e);
    }

    vector<vector<int>> dp(cells.size() + 1, vector<int>(cells.size() + 1, INT_MAX));
    fill(dp[1].begin(), dp[1].end(), 0);
    for (int i = 2; i <= cells.size(); i++)
        for (int j = 0; j + i <= cells.size(); j++)
            for (int k = j + 1; k < j + i; k++)
                dp[i][j] = min(dp[i][j], (cells[j] != cells[k]) + dp[k - j][j] + dp[i - k + j][k]);

    cout << dp.back()[0];
}
