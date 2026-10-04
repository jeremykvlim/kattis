#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c, q;
    cin >> r >> c >> q;

    vector<int> dp((r + 1) * (c + 1)), height(c);
    auto index = [&](int i, int j) {
        return i * (c + 1) + j;
    };

    for (int i = 0; i < r; i++) {
        string row;
        cin >> row;

        stack<pair<int, int>> s;
        s.emplace(-1, 0);
        for (int j = 0; j < c; j++) {
            if (row[j] != '#') height[j]++;
            else height[j] = 0;

            while (height[j] < s.top().first) {
                auto [hk, k] = s.top();
                s.pop();
                auto [hl, l] = s.top();
                dp[index(hk, j - l)]++;
                dp[index(max(height[j], hl), j - l)]--;
            }
            s.emplace(height[j], j + 1);
        }

        while (s.top().second) {
            auto [hk, k] = s.top();
            s.pop();
            auto [hl, l] = s.top();
            dp[index(hk, c - l)]++;
            dp[index(max(0, hl), c - l)]--;
        }
    }

    vector<int> suff(c + 1, 0);
    for (int i = r; i; i--)
        for (int j = c, sum = 0, count = 0; j; j--) dp[index(i, j)] = count += sum += suff[j] += dp[index(i, j)];

    while (q--) {
        int h, w;
        cin >> h >> w;
        cout << dp[index(h, w)] << "\n";
    }
}