#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (!n) {
        cout << 0;
        exit(0);
    }

    vector<int> y(n + 1, 0), z(n + 1, 0);
    vector<long long> t(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> z[i] >> t[i];

        y[i] = z[i - 1];
    }
    y[n] = z[n - 1];

    vector<pair<int, int>> intervals;
    for (int a = 0; a <= 10; a++)
        for (int b = a; b <= 10; b++) intervals.emplace_back(a, b);

    vector<int> l(intervals.size(), 0);
    vector<long long> dp(n + 1, 1e18);
    dp[0] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < intervals.size(); j++) {
            auto [a, b] = intervals[j];
            int &k = l[j];

            if (a <= y[i] && y[i] <= b && a <= z[i] && z[i] <= b) {
                for (; k < i && (!(a <= y[k + 1] && y[k + 1] <= b && a <= z[k + 1] && z[k + 1] <= b) || t[i] - t[k + 1] > 1e4); k++);
                if (k < i) dp[i] = min(dp[i], dp[k] + 2 + b - a);
            } else k = i;
        }
    cout << dp[n];
}
