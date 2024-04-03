#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> activities(n), lis(n, 0), dp(n + 1, 0);
    vector<vector<pair<int, int>>> day(n + 1);
    dp[0] = 1;
    day[0].emplace_back(0, 1);
    int len = 1;
    for (int &a : activities) {
        cin >> a;

        int i = a > lis[len - 1] ? len++ : lower_bound(lis.begin(), lis.begin() + len, a) - lis.begin();
        lis[i] = a;

        int l = -1, r = day[i - 1].size() - 1, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;
            
            if (day[i - 1][m].first < a) r = m;
            else l = m;
        }

        if (!day[i].empty() && day[i].back().first == a) {
            auto p = day[i].back();
            day[i].pop_back();
            dp[i] = day[i].empty() ? 0 : (dp[i] - p.second + day[i].back().second + MODULO) % MODULO;
        }

        dp[i] = (dp[i] + (l == -1 ? dp[i - 1] : (dp[i - 1] - day[i - 1][l].second + MODULO) % MODULO)) % MODULO;
        day[i].emplace_back(a, dp[i]);
    }

    cout << dp[len - 1];
}
