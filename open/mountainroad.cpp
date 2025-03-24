#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C;
    cin >> C;

    while (C--) {
        int n;
        cin >> n;

        array<vector<pair<int, int>>, 2> cars;
        while (n--) {
            char c;
            int t, d;
            cin >> c >> t >> d;

            cars[c - 'A'].emplace_back(t, d);
        }

        int A = cars[0].size(), B = cars[1].size();
        vector<vector<vector<int>>> dp(A + 1, vector<vector<int>>(B + 2, vector<int>(2, INT_MAX)));
        dp[0][0][0] = dp[0][0][1] = 0;
        for (int i = 0; i <= A; i++)
            for (int j = 0; j <= B; j++)
                for (int k = 0; k < 2; k++) {
                    int l = !k ? i : j, r = !k ? A : B;

                    if (dp[i][j][k ^ 1] == INT_MAX) continue;
                    int prev_l = dp[i][j][k ^ 1], prev_r = INT_MIN;
                    for (int m = l; m < r; m++) {
                        int curr_l = max(prev_l, cars[k][m].first), curr_r = max(curr_l + cars[k][m].second, prev_r + 10);
                        prev_l = curr_l + 10;
                        prev_r = curr_r;

                        int a = !k ? m + 1 : i, b = !k ? j : m + 1;
                        dp[a][b][k] = min(dp[a][b][k], curr_r);
                    }
                }

        cout << min(dp[A][B][0], dp[A][B][1]) << "\n";
    }
}
