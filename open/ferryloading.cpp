#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int ferry;
    cin >> ferry;
    ferry *= 100;

    vector<vector<int>> dp(501, vector<int>(1e5 + 1, -1));
    dp[0][0] = 0;
    int car, sum = 0, count = 0;
    while (cin >> car && car) {
        sum += car;

        bool done = true;
        for (int j = 0; j <= 1e5; j++)
            if (dp[count][j] != -1) {
                if (j + car <= ferry && sum - car - j <= ferry) {
                    dp[count + 1][j + car] = j;
                    done = false;
                }

                if (j <= ferry && sum - j <= ferry) {
                    dp[count + 1][j] = j;
                    done = false;
                }
            }

        if (done) break;
        count++;
    }
    cout << count << "\n";

    int j = 0;
    while (dp[count][j] == -1) j++;
    auto dfs = [&](auto &&self, int count, int j) {
        if (!count) return;

        self(self, count - 1, dp[count][j]);
        cout << (j == dp[count][j] ? "starboard" : "port") << "\n";
    };
    dfs(dfs, count, j);
}
