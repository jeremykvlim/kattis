#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> h(n);
    for (int &hi : h) cin >> hi;

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return h[i] != h[j] ? h[i] > h[j] : i < j; });

    int zip = 0;
    set<int> active;
    vector<int> dp(n, 0);
    for (int i = 0; i < n;) {
        int hi = h[order[i]];
        vector<int> indices;
        for (; i < n && h[order[i]] == hi; i++) indices.emplace_back(order[i]);

        for (int j : indices) active.emplace(j);
        for (int j : indices) {
            auto it = active.find(j);
            if (it != active.begin()) {
                int k = *prev(it);
                if (h[k] > hi) dp[j] = max(dp[j], dp[k] + 1);
            }
            if (next(it) != active.end()) {
                int k = *next(it);
                if (h[k] > hi) dp[j] = max(dp[j], dp[k] + 1);
            }
            zip = max(zip, dp[j]);
        }
    }
    cout << zip;
}
