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
        for (int i = 0; i <= 1e5; i++)
            if (dp[count][i] != -1) {
                if (i + car <= ferry && sum - car - i <= ferry) {
                    dp[count + 1][i + car] = i;
                    done = false;
                }

                if (i <= ferry && sum - i <= ferry) {
                    dp[count + 1][i] = i;
                    done = false;
                }
            }

        if (done) break;
        count++;
    }
    cout << count << "\n";

    int i = 0;
    while (dp[count][i] == -1) i++;
    auto dfs = [&](auto &&self, int count, int i) {
        if (!count) return;

        self(self, count - 1, dp[count][i]);
        cout << (dp[count][i] == i ? "starboard" : "port") << "\n";
    };
    dfs(dfs, count, i);
}
