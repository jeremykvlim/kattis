#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<pair<int, int>> videos(n);
    for (auto &[a, b] : videos) cin >> a >> b;

    auto max_k = 0LL;
    vector<long long> k(q);
    for (auto &ki : k) {
        cin >> ki;

        max_k = max(max_k, ki);
    }
    sort(videos.begin(), videos.end(), [&](auto &p1, auto &p2) {
             auto f1 = (double) (p1.first - 1) / p1.second, f2 = (double) (p2.first - 1) / p2.second;
             if (f1 != f2) return f1 < f2;
             return p1.second > p2.second;
         }
    );

    vector<long long> dp{0};
    for (auto [a, b] : videos) {
        dp.emplace_back(a * dp.back() + b);

        if (a > 1)
            for (int i = dp.size() - 2; i; i--) dp[i] = max(dp[i], a * dp[i - 1] + b);

        while (dp.size() > 2 && dp[dp.size() - 2] >= max_k) dp.pop_back();
    }

    for (auto ki : k) {
        int i = lower_bound(dp.begin(), dp.end(), ki) - dp.begin();
        cout << (i == dp.size() ? -1 : i) << " ";
    }
}
