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

    vector<pair<int, int>> intervals;
    for (auto [u, v] : enemies) {
        while (!intervals.empty() && intervals.back().second >= v) intervals.pop_back();
        if (intervals.empty() || intervals.back().first < u) intervals.emplace_back(u, v);
    }

    int k = intervals.size();
    vector<int> dp(k, 0);
    deque<pair<int, int>> mono;
    for (int i = 0, j = 0; i < n - 1; i++) {
        for (; j < k && intervals[j].second <= i; j++);

        int value = c[i] + (j ? dp[j - 1] : 0);
        while (!mono.empty() && mono.back().second >= value) mono.pop_back();
        mono.emplace_back(i, value);
        if (j < k && intervals[j].second == i + 1) {
            while (mono.front().first < intervals[j].first) mono.pop_front();
            dp[j] = mono.front().second;
        }
    }
    cout << dp[k - 1];
}
