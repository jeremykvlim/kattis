#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> c(n - 1);
    for (int &ci : c) cin >> ci;

    int m;
    cin >> m;

    vector<pair<int, int>> enemies(m);
    for (auto &[u, v] : enemies) {
        cin >> u >> v;
        u--;
        v--;
    }
    sort(enemies.begin(), enemies.end());

    deque<pair<int, int>> mono_inc;
    for (auto [u, v] : enemies) {
        while (mono_inc.size() && mono_inc.back().second >= v) mono_inc.pop_back();
        if (mono_inc.empty() || mono_inc.back().first < u) mono_inc.emplace_back(u, v);
    }

    vector<int> dp(mono_inc.size(), 1e9);
    multiset<int> ms;
    for (int i = 0, l = 0, r = 0; i < n - 1; i++) {
        if (r < mono_inc.size() && mono_inc[r].first == i) ms.emplace(dp[r++]);
        if (l < r) {
            ms.erase(ms.find(dp[r - 1]));
            dp[r - 1] = min(dp[r - 1], c[i] + (!l ? 0 : dp[l - 1]));
            ms.emplace(dp[r - 1]);

            if (mono_inc[l].second == i + 1) {
                ms.erase(ms.find(dp[l]));
                if (!ms.empty()) dp[l] = min(dp[l], *ms.begin());
                l++;
            }
        }
    }
    cout << dp.back();
}
