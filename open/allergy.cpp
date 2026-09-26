#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<int> count(8, 0);
    while (k--) {
        int d;
        cin >> d;

        count[d]++;
    }

    int total = 1;
    vector<int> base(8, 1);
    for (int d = 1; d <= 7; d++) {
        base[d] = total;
        total *= count[d] + 1;
    }

    vector<vector<int>> dp(total, vector<int>(8, 1e9));
    dp[0][1] = 0;
    for (int i = 0; i < total; i++)
        for (int d1 = 1; d1 <= 7; d1++)
            if ((i / base[d1]) % (count[d1] + 1) != count[d1])
                for (int j = i + base[d1], d2 = 1; d2 <= 7; d2++)
                    if (dp[i][d2] != 1e9) {
                        int d3 = max(1, d1 - d2 + 1);
                        dp[j][d3] = min(dp[j][d3], dp[i][d2] + d3);
                    }
    cout << *min_element(dp.back().begin(), dp.back().end());
}
