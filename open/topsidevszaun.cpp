#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    if (n == 1) {
        cout << -1;
        exit(0);
    }

    sort(a.rbegin(), a.rend());
    mt19937_64 rng(random_device{}());
    int fighters = 0, i = min((int) (sqrt(n) + 1), n / 2), j = i * a[0], m = 2 * i + 1, k = 2 * j + 1;
    vector<vector<int>> dp(m, vector<int>(k, -1e9)), temp_dp(m, vector<int>(k, -1e9));
    vector<int> most(m, -1e9), temp_most(m, -1e9);
    while (fighters < n - (n & 1)) {
        for (auto &row : dp) fill(row.begin(), row.end(), -1e9);
        fill(most.begin(), most.end(), -1e9);
        dp[i][j] = most[i] = 0;

        int count = 0;
        for (int ai : a) {
            for (auto &row : temp_dp) fill(row.begin(), row.end(), -1e9);
            fill(temp_most.begin(), temp_most.end(), -1e9);

            int remaining = n - ++count;
            for (int x = 0; x < m; x++) {
                if (most[x] + remaining <= fighters) continue;

                for (int y = 0; y < k; y++) {
                    if (dp[x][y] < 0) continue;

                    temp_dp[x][y] = max(temp_dp[x][y], dp[x][y]);
                    temp_most[x] = max(temp_most[x], dp[x][y]);

                    if (x + 1 < m && y + ai < k)
                        if (temp_dp[x + 1][y + ai] < dp[x][y] + 1) {
                            temp_dp[x + 1][y + ai] = dp[x][y] + 1;
                            temp_most[x + 1] = max(temp_most[x + 1], dp[x][y] + 1);
                        }

                    if (x && y >= ai)
                        if (temp_dp[x - 1][y - ai] < dp[x][y] + 1) {
                            temp_dp[x - 1][y - ai] = dp[x][y] + 1;
                            temp_most[x - 1] = max(temp_most[x - 1], dp[x][y] + 1);
                        }
                }
            }

            dp = temp_dp;
            most = temp_most;
        }

        if (fighters > dp[i][j]) break;
        fighters = dp[i][j];
        shuffle(a.begin(), a.end(), rng);
    }

    cout << (!fighters ? -1 : fighters / 2);
}
