#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, long long>> components(n);
    int most = 0;
    for (auto &[f, t] : components) {
        cin >> f >> t;

        most = max(most, f);
    }
    sort(components.begin(), components.end(), [&](auto p1, auto p2) { return p1.second > p2.second; });

    int total = 0;
    vector<long long> dp(2 * most + 1, 1e16);
    dp[0] = 0;
    for (auto [f, t] : components) {
        vector<long long> temp(2 * most + 1, 1e16);
        for (int i = (total + f) & 1, copies = 0; i <= 2 * most; i += 2) {
            auto time = [&](int c) {
                if (i < 2 * c - f) return LLONG_MAX;
                return dp[i - 2 * c + f] + t * c;
            };

            while (i - 2 * copies + f > total) copies++;
            if (copies > min(i, f)) continue;
            while (copies + 1 <= min(i, f) && time(copies + 1) <= time(copies)) copies++;
            temp[i] = time(copies);
        }
        total += f;
        dp = temp;
    }
    cout << *min_element(dp.begin(), dp.end());
}
