#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, C;
        cin >> n >> C;

        vector<int> values(n), costs(n);
        for (int &v : values) cin >> v;
        for (int &c : costs) cin >> c;

        int V = accumulate(values.begin(), values.end(), 0), loot = -1;
        if (V > 1e6) {
            vector<pair<int, int>> chosen;
            set<pair<int, int>> s;
            for (int i = 0; i < 1 << (n / 2); i++) {
                int total_v = 0, total_c = 0;
                for (int j = 0; j < n / 2; j++)
                    if (i & (1 << j)) {
                        total_v += values[j];
                        total_c += costs[j];
                    }

                if (total_c <= C) {
                    chosen.emplace_back(total_c, total_v);
                    loot = max(loot, total_v);
                }
            }
            sort(chosen.begin(), chosen.end());

            s.emplace(chosen[0]);
            for (int i = 1; i < chosen.size(); i++) {
                chosen[i].second = max(chosen[i].second, chosen[i - 1].second);
                s.emplace(chosen[i]);
            }

            for (int i = 0; i < 1 << (n - (n / 2)); i++) {
                int total_v = 0, total_c = 0;
                for (int j = 0; j < n - n / 2; j++)
                    if (i & (1 << j)) {
                        total_v += values[j + n / 2];
                        total_c += costs[j + n / 2];
                    }

                if (total_c <= C) {
                    auto it = s.lower_bound(make_pair(C - total_c + 1, 0));
                    while (it == s.end() || it->first + total_c > C) it--;
                    loot = max(loot, it->second + total_v);
                }
            }
        } else {
            vector<vector<int>> dp(2, vector<int>(V + 1, 0));
            fill(dp[0].begin() + 1, dp[0].end(), C + 1);

            for (int i = 1; i <= n; i++)
                for (int j = 0; j <= V; j++) {
                    dp[i & 1][j] = dp[(i - 1) & 1][j];
                    if (j >= values[i - 1]) dp[i & 1][j] = min(dp[i & 1][j], dp[(i - 1) & 1][j - values[i - 1]] + costs[i - 1]);
                    if (dp[i & 1][j] <= C) loot = max(loot, j);
                }
        }

        cout << loot << "\n";
    }
}
