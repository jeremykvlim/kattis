#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, C;
    cin >> n >> C;

    vector<pair<int, int>> items(n);
    for (auto &[v, w] : items) cin >> v >> w;
    sort(items.begin(), items.end(), [&](auto &p1, auto &p2) { return (long long) p1.first * p2.second > (long long) p2.first * p1.second; });

    int i = 0;
    long long v1 = 0, w1 = 0;
    for (; i < n && w1 + items[i].second <= C; i++) {
        v1 += items[i].first;
        w1 += items[i].second;
    }

    if (i == n) {
        cout << v1;
        exit(0);
    }

    auto [vi, wi] = items[i];
    auto v2 = v1, w2 = w1;
    for (; i < n; i++)
        if (w2 + items[i].second <= C) {
            v2 += items[i].first;
            w2 += items[i].second;
        }

    auto deficit = vi * (C - w1) + (v1 - v2) * wi, base = 0LL;
    vector<pair<int, int>> valid;
    for (auto [v, w] : items) {
        auto d = (long long) v * wi - (long long) vi * w;
        if (d > deficit) {
            base += v;
            C -= w;
        } else if (d + deficit >= 0) valid.emplace_back(v, w);
    }

    vector<long long> dp(C + 1, 0);
    for (auto [v, w] : valid)
        for (int c = C; c >= w; c--) dp[c] = max(dp[c], dp[c - w] + v);
    cout << base + dp[C];
}
