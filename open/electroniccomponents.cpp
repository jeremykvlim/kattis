#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, long long>> components(n);
    int most = -1;
    for (auto &[f, t] : components) {
        cin >> f >> t;

        most = max(most, f);
    }
    sort(components.begin(), components.end(), [&](pair<int, int> p1, pair<int, int> p2) {return p1.second > p2.second;});

    vector<vector<pair<int, long long>>> placements(most + 1);
    vector<long long> dp(most + 1, LLONG_MAX);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        auto [f, t] = components[i];
        for (int j = 0; j < dp.size(); j++)
            if (dp[j] != LLONG_MAX) placements[f + j - 2 * min(f, j)].emplace_back(f + j, dp[j] + (f - min(j, f)) * t);

        for (int j = 0; j < 2; j++) {
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
            for (int k = j; k < dp.size(); k += 2) {
                while (!pq.empty() && pq.top().second < k) pq.pop();

                for (auto &p : placements[k]) pq.emplace(p.second - k / 2 * t, p.first);
                placements[k].clear();
                dp[k] = !pq.empty() ? pq.top().first + k / 2 * t : LLONG_MAX;
            }
        }
    }

    cout << *min_element(dp.begin(), dp.end());
}
