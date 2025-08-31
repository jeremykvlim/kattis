#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<long long> pref(n, 0);
    for (int i = 0; i < n - 1; i++) {
        int d;
        cin >> d;

        pref[i + 1] = pref[i] + d;
    }

    vector<int> dp(n);
    long long diff = 1e18;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++) {
            auto b = pref[i] - pref[j], l = b - 1, r = min(diff + b, pref[n - 1]);

            auto build = [&](vector<long long> pref, auto a) {
                fill(dp.begin(), dp.end(), 0);
                int sites = 1;
                priority_queue<pair<int, int>> pq;
                for (int i = 0, j = 0; i < pref.size(); i++) {
                    while (i > j && pref[i] - pref[j] >= b) {
                        pq.emplace(dp[j], j);
                        j++;
                    }

                    while (!pq.empty() && pref[i] - pref[pq.top().second] > a) pq.pop();
                    sites = max(sites, dp[i] = pq.empty() ? 1 : pq.top().first + 1);
                }

                return sites;
            };

            if (build(pref, r) < k) continue;

            while (l + 1 < r) {
                auto m = l + (r - l) / 2;

                if (build(pref, m) >= k) r = m;
                else l = m;
            }
            diff = min(diff, r - b);
        }
    cout << diff;
}
