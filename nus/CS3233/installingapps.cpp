#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<pair<pair<int, int>, int>> apps(n);
    for (int i = 0; i < n; i++) {
        cin >> apps[i].first.first >> apps[i].first.second;
        apps[i].second = i;
    }

    sort(apps.begin(), apps.end(), [&](auto i, auto j) {
        return i.first.second - i.first.first == j.first.second - j.first.first ? i.second < j.second : i.first.second - i.first.first < j.first.second - j.first.first;
    });

    vector<vector<int>> prev(n + 1, vector<int>(n + 1, -1));
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, INT_MAX));
    dp[0][0] = 0;
    for (int i = 0; i < n; i++) {
        dp[i + 1] = dp[i];
        for (int j = 0; j <= i; j++)
            if (dp[i][j] + max(apps[i].first.first, apps[i].first.second) <= c && dp[i][j] + apps[i].first.second < dp[i + 1][j + 1]) {
                dp[i + 1][j + 1] = dp[i][j] + apps[i].first.second;
                prev[i + 1][j + 1] = i;
            }
    }

    for (int i = n; ~i; i--)
        if (dp[n][i] != INT_MAX) {
            cout << i << "\n";

            stack<int> install;
            for (int j = n, k = i; j; j--)
                if (prev[j][k] != -1) install.emplace(apps[prev[j][k--]].second + 1);

            while (!install.empty()) {
                cout << install.top() << " ";
                install.pop();
            }
            
            break;
        }
}
