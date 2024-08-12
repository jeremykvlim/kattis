#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u - 1].emplace_back(v - 1, w);
        adj_list[v - 1].emplace_back(u - 1, w);
    }

    vector<vector<long long>> dist(n, vector<long long>(n, LLONG_MAX));
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        pq.emplace(0, i);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[i][v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[i][u] > d + w) {
                    dist[i][u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
    }

    int k;
    cin >> k;

    vector<array<int, 3>> orders(k);
    for (auto &[s, u, t] : orders) {
        cin >> s >> u >> t;

        u--;
    }

    long long l = -1, r = 1e14, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        auto valid = [&](auto wait) -> bool {
            vector<long long> dp(k + 1, 1e18);
            dp[0] = 0;
            for (int i = 0; i < k; i++) {
                auto d = 0LL, latest = dp[i], longest = LLONG_MIN;
                for (int j = i, v = 0; j < k; j++) {
                    auto [s, u, t] = orders[j];

                    d += dist[v][u];
                    latest = max(latest, (long long) t);
                    longest = max(longest, d - s);
                    if (longest + latest > wait) break;

                    dp[j + 1] = min(dp[j + 1], latest + d + dist[u][0]);
                    v = u;
                }
            }
            
            return dp[k] != 1e18;
        };


        if (valid(mid)) r = mid;
        else l = mid;
    }

    cout << (l == -1 ? 0 : r);
}
