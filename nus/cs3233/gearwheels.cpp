#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> teeth(n);
    for (auto &t : teeth) cin >> t;

    auto ratio = [&](int i, int j) -> bool {
        return !(teeth[i] % teeth[j]) || !(teeth[j] % teeth[i]);
    };

    vector<int> dp(n, 1);
    int l = -1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            if (ratio(i, j)) {
                dp[i] = max(dp[i], dp[j] + 1);
                l = max(l, dp[i]);
            }

    vector<bool> visited(n, false);
    vector<int> indices;
    vector<vector<int>> chains;
    auto dfs = [&](auto &&self, int i) -> bool {
        if (visited[i]) return false;

        indices.emplace_back(i);
        if (dp[i] == 1) {
            chains.emplace_back(indices);
            done:;
            indices.pop_back();
            visited[i] = true;
            return true;
        }

        for (int j = i - 1; ~j; j--)
            if (ratio(i, j) && dp[i] == dp[j] + 1)
                if (self(self, j)) goto done;

        indices.pop_back();
        return false;
    };
    for (int i = 0; i < n; i++)
        if (dp[i] == l) dfs(dfs, i);

    cout << l << " " << chains.size() << "\n";
    for (auto &c : chains) {
        reverse(c.begin(), c.end());
        for (int i : c) cout << i + 1 << " ";
        cout << "\n";
    }
}
