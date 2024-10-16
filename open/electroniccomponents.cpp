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
    sort(components.begin(), components.end(), [&](auto p1, auto p2) {return p1.second > p2.second;});

    vector<vector<pair<int, long long>>> placements(most + 1);
    vector<long long> dp(most + 1, LLONG_MAX);
    dp[0] = 0;
    for (auto [fi, ti] : components) {
        for (int i = 0; i < dp.size(); i++)
            if (dp[i] != LLONG_MAX) placements[fi + i - 2 * min(fi, i)].emplace_back(fi + i, dp[i] + (fi - min(fi, i)) * ti);

        for (int i = 0; i < 2; i++) {
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
            for (int j = i; j <= most; j += 2) {
                while (!pq.empty() && pq.top().second < j) pq.pop();

                for (auto [fj, tj] : placements[j]) pq.emplace(tj - j / 2 * ti, fj);
                placements[j].clear();
                dp[j] = !pq.empty() ? pq.top().first + j / 2 * ti : LLONG_MAX;
            }
        }
    }

    cout << *min_element(dp.begin(), dp.end());
}
