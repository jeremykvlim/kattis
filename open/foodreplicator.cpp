#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, f;
    cin >> m >> n >> f;

    vector<int> c(n);
    for (int &ci : c) cin >> ci;

    vector<vector<int>> tokens(n);
    while (f--) {
        int p, t;
        cin >> p >> t;

        tokens[p].emplace_back(t);
    }

    vector<array<int, 3>> batches;
    for (int i = 0; i < n; i++)
        if (!tokens[i].empty()) {
            sort(tokens[i].begin(), tokens[i].end());
            for (int j = 0, prev = 0; j < tokens[i].size(); j++) {
                int len = tokens[i][j] - prev;
                if (len > 0) {
                    int count = min(len, m / c[i]);
                    if (c[i] <= m && count) batches.push_back({c[i], (int) tokens[i].size() - j, count});
                }
                prev = tokens[i][j];
            }
        }

    vector<int> dp(m + 1, 0);
    for (auto [ci, h, count] : batches) {
        auto temp = dp;
        for (int r = 0; r <= min(m, ci - 1); r++) {
            deque<pair<int, int>> dq;
            for (int q = 0; q <= (m - r) / ci; q++) {
                int i = r + q * ci, bound = temp[i] - q * h;
                while (!dq.empty() && dq.back().second <= bound) dq.pop_back();
                dq.emplace_back(q, bound);
                while (!dq.empty() && dq.front().first < q - count) dq.pop_front();
                dp[i] = dq.front().second + q * h;
            }
        }
    }
    cout << *max_element(dp.begin(), dp.end());
}
