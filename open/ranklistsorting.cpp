#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> scores(n);
    for (int i = 0; i < n; i++) {
        cin >> scores[i].first;

        scores[i].second = i;
    }
    sort(scores.rbegin(), scores.rend());

    vector<int> rank(n + 1, n), pos(n + 1);
    for (int i = 0; i < n; i++) {
        rank[scores[i].second] = i;
        pos[i] = scores[i].second;
    }

    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0)), prev(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i < n; i++) dp[n][i] = 1e9;

    for (int i = n - 1; ~i; i--) {
        int count = count_if(rank.begin(), rank.begin() + pos[i], [i](auto r) {return i > r;}) + 2;

        for (int j = 0; j <= n; j++) {
            if (i > rank[j]) count++;

            dp[i][j] = dp[i + 1][j] + count;
            prev[i][j] = j;
        }

        for (int j = pos[i] + 1, k = 0; j <= n; j++) {
            if (dp[i][pos[i]] > dp[i + 1][j] + k) {
                dp[i][pos[i]] = dp[i + 1][j] + k;
                prev[i][pos[i]] = j;
            }

            if (i > rank[j]) k += i - rank[j];
        }
    }

    vector<bool> same(n + 1, true);
    for (int i = 0, j = min_element(dp[0].begin(), dp[0].end()) - dp[0].begin(); i < n; i++) {
        same[i] = pos[i] == j;
        j = prev[i][j];
    }

    vector<pair<int, int>> moves;
    for (int k = n - 1; ~k; k--) {
        if (same[k]) continue;

        auto it1 = find(rank.begin(), rank.end(), k);
        int i = it1 - rank.begin() + 1;
        rank.erase(it1);

        auto it2 = find(rank.begin(), rank.end(), k + 1);
        int j = it2 - rank.begin() + 1;
        rank.emplace(it2, k);

        moves.emplace_back(i, j);
    }

    cout << moves.size() << "\n";
    for (auto [i, j] : moves) cout << i << " " << j << "\n";
}
