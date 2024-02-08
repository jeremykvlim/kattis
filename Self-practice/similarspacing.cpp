#include <bits/stdc++.h>
using namespace std;

int build(vector<long long> pref, long long a, long long b) {
    vector<int> dp(pref.size(), 0);
    priority_queue<pair<int, int>> pq;
    int sites = 1;
    for (int i = 0, j = 0; i < pref.size(); i++) {
        while (i > j && pref[i] - pref[j] >= b) {
            pq.emplace(dp[j], j);
            j++;
        }

        while (!pq.empty() && pref[i] - pref[pq.top().second] > a) pq.pop();

        dp[i] = pq.empty() ? 1 : pq.top().first + 1;
        sites = max(sites, dp[i]);
    }

    return sites;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> dist(n - 1);
    vector<long long> pref(n, 0);
    for (int i = 0; i < n - 1; i++) {
        cin >> dist[i];

        pref[i + 1] = pref[i] + dist[i];
    }

    long long diff = 1e18;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++) {
            auto b = pref[i] - pref[j], l = b - 1, r = min(diff + b, pref[n - 1]);

            if (build(pref, r, b) < k) continue;

            while (l + 1 < r) {
                auto m = l + (r - l) / 2;

                if (build(pref, m, b) >= k) r = m;
                else l = m;
            }

            diff = min(diff, r - b);
        }

    cout << diff;
}
