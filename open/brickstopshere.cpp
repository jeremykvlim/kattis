#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> dp(51, vector<int>((50 * 999) + 1, 1e9));
    dp[0][0] = 0;
    while (n--) {
        int cu, p;
        cin >> cu >> p;

        for (int i = 50; i; i--)
            for (int j = i * 999; j >= cu; j--)
                if (dp[i - 1][j - cu] != 1e9) dp[i][j] = min(dp[i][j], dp[i - 1][j - cu] + p);
    }

    int c;
    cin >> c;

    while (c--) {
        int m, cmin, cmax;
        cin >> m >> cmin >> cmax;

        int price = 1e9;
        for (int j = m * cmin; j <= m * cmax; j++) price = min(price, dp[m][j]);

        if (price == 1e9) cout << "impossible\n";
        else cout << price << "\n";
    }
}
