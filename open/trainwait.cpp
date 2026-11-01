#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    vector<pair<int, int>> trains(t);
    vector<array<int, 3>> events(2 * t);
    for (int i = 0; i < t; i++) {
        auto &[u, v] = trains[i];
        int dep, arr;
        cin >> u >> v >> dep >> arr;

        events[2 * i] = {dep, 1, i};
        events[2 * i + 1] = {arr, 0, i};
    }
    sort(events.begin(), events.end());

    auto sum = LLONG_MAX;
    vector<long long> dp1(n + 1, 1e18), dp2(t, 1e18);
    dp1[1] = 0;
    for (auto [time, type, i] : events) {
        auto [u, v] = trains[i];

        if (!type) {
            if (dp2[i] != 1e18) {
                dp1[v] = min(dp1[v], dp2[i] - time);
                if (v == n) sum = min(sum, dp2[i]);
            }
        } else if (dp1[u] != 1e18) dp2[i] = dp1[u] + time;
    }
    cout << (sum == LLONG_MAX ? -1 : sum);
}
