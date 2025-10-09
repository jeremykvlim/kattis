#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x, y;
    double h, s;
    cin >> n >> x >> y >> h >> s;
    h = 1. / h;
    s = 1. / s;

    vector<int> positions(n);
    for (int &pos : positions) cin >> pos;
    positions.emplace_back(0);
    positions.emplace_back(42195);
    sort(positions.begin(), positions.end());
    positions.erase(unique(positions.begin(), positions.end()), positions.end());

    int p = positions.size();
    vector<double> dp(p, 1e20);
    dp[0] = 0;
    deque<int> mono{0};
    auto least = 1e20;
    for (int l = 0, r = 1; r < p; r++) {
        for (; l < r && positions[l] <= positions[r] - x; l++) least = min(least, dp[l] + (h - s) * x - s * positions[l]);

        while (!mono.empty() && positions[mono.front()] < positions[r] - x) mono.pop_front();

        auto cost = [&](int i) {
            return dp[i] - h * positions[i];
        };

        if (!mono.empty()) dp[r] = min(dp[r], h * positions[r] + cost(mono.front()));
        dp[r] = min(dp[r], s * positions[r] + least);
        if (r < p - 1) dp[r] += y;

        auto c = cost(r);
        while (!mono.empty() && cost(mono.back()) >= c) mono.pop_back();
        mono.emplace_back(r);
    }

    long long seconds = floor(dp[p - 1] + 1e-9);
    cout << setfill('0') << setw(2) << seconds / 3600 << ":" << setw(2) << (seconds % 3600) / 60 << ":" << setw(2) << seconds % 60;
}
