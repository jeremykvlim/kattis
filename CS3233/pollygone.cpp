#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double P;
    cin >> n >> P;

    vector<int> dp(10001, INT_MAX);
    dp[0] = 0;
    int j = 0;
    while (n--) {
        int e;
        double p;
        cin >> e >> p;

        int prob = int(round(p * 10000));
        for (int i = j; ~i; i--)
            if (dp[i + prob] > dp[i] + e) {
                dp[i + prob] = dp[i] + e;
                j = max(j, i + prob);
            }
    }

    cout << *min_element(dp.begin() + int(round(P * 10000)), dp.end());
}
