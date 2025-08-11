#include <bits/stdc++.h>
using namespace std;

template <typename T>
tuple<vector<int>, T, vector<vector<T>>> held_karp(int n, const vector<vector<T>> &dist, int src = 0) {
    T inf = numeric_limits<T>::max() >> 2;
    vector<vector<T>> dp(1 << n, vector<T>(n, inf));
    vector<vector<int>> prev(1 << n, vector<int>(n, -1));

    for (int i = 0; i < n; i++) dp[1 << i][i] = dist[src][i + 1];

    for (int m1 = 1; m1 < 1 << n; m1++)
        for (int i = 0; i < n; i++)
            if ((m1 >> i) & 1)
                if (dp[m1][i] != inf)
                    for (int m2 = m1 ^ ((1 << n) - 1); m2; m2 &= m2 - 1) {
                        int j = countr_zero((unsigned) m2), m3 = m1 | (1 << j);
                        if (dp[m3][j] > dp[m1][i] + dist[i + 1][j + 1]) {
                            dp[m3][j] = dp[m1][i] + dist[i + 1][j + 1];
                            prev[m3][j] = i;
                        }
                    }

    T len = inf;
    int j = -1;
    for (int i = 0; i < n; i++)
        if (len > dp.back()[i] + dist[i + 1][src]) {
            len = dp.back()[i] + dist[i + 1][src];
            j = i;
        }

    int mask = (1 << n) - 1;
    vector<int> tour;
    while (mask) {
        tour.emplace_back(j + 1);
        int k = prev[mask][j];
        mask ^= 1 << j;
        j = k;
    }
    tour.emplace_back(src);
    reverse(tour.begin(), tour.end());

    return {tour, len, dp};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, e, k, t, l, a_max;
    cin >> n >> e >> k >> t >> l >> a_max;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (e--) {
        int u, v, w;
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<int> quests(k), spells(l);
    for (int &q : quests) cin >> q;
    for (int &s : spells) cin >> s;
    quests.insert(quests.begin(), 1);

    vector<long long> dist(n + 1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    vector<vector<long long>> d(k + 1, vector<long long>(k + 1, 1e18));
    for (int i = 0; i <= k; i++) {
        fill(dist.begin(), dist.end(), 1e18);
        dist[quests[i]] = 0;
        pq.emplace(0, quests[i]);
        while (!pq.empty()) {
            auto [dv, v] = pq.top();
            pq.pop();

            if (dist[v] != dv) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > dv + w) {
                    dist[u] = dv + w;
                    pq.emplace(dist[u], u);
                }
        }
        for (int j = 0; j <= k; j++) d[i][j] = dist[quests[j]];
    }

    if (!l) {
        auto [tour, len, dp] = held_karp(k, d);
        cout << len;
        exit(0);
    }

    vector<int> indices(n + 1);
    for (int i = 0; i < k; i++) indices[quests[i + 1]] = i;

    int spell_mask = 0;
    for (int c : spells) spell_mask |= 1 << indices[c];

    vector<vector<long long>> cost1(k + 1, vector<long long>(1 << k, 1e18));
    for (int src = 0; src <= k; src++) {
        auto [tour, len, dp] = held_karp(k, d, src);

        for (auto m1 = 1U; m1 < 1 << k; m1++)
            for (auto m2 = m1 & spell_mask; m2; m2 &= m2 - 1) {
                int i = countr_zero(m2);
                cost1[src][m1] = min(cost1[src][m1], dp[m1][i]);
            }
    }

    vector<int> popcount(1 << k, 0);
    for (int mask = 1; mask < 1 << k; mask++) popcount[mask] = popcount[mask >> 1] + (mask & 1);

    vector<vector<long long>> cost2(k + 1, vector<long long>(1 << k, 1e18));
    vector<vector<vector<long long>>> cost3(k + 1, vector<vector<long long>>(1 << k, vector<long long>(k, 1e18)));
    for (int src = 0; src <= k; src++)
        for (auto m1 = 0U; m1 < 1 << k; m1++) {
            if (src && ((1 << (src - 1)) & spell_mask)) {
                int a = popcount[m1];
                if (a <= a_max) {
                    if (a < a_max) cost2[src][m1] = min(cost2[src][m1], (long long) t);
                    for (auto m2 = m1; m2; m2 &= m2 - 1) {
                        int i = countr_zero(m2);
                        cost3[src][m1][i] = min(cost3[src][m1][i], (long long) t);
                    }
                }
            }

            for (auto m2 = m1; m2; --m2 &= m1)
                if (m2 & spell_mask)
                    if (cost1[src][m2] != 1e18) {
                        cost1[src][m2] = max(cost1[src][m2], (long long) t);
                        int a = popcount[m1] - popcount[m2];
                        if (a <= a_max) {
                            if (a < a_max) cost2[src][m1] = min(cost2[src][m1], cost1[src][m2]);
                            for (auto m3 = m1 & ~m2; m3; m3 &= m3 - 1) {
                                int i = countr_zero(m3);
                                cost3[src][m1][i] = min(cost3[src][m1][i], cost1[src][m2]);
                            }
                        }
                    }
        }

    vector<vector<long long>> dp(1 << k, vector<long long>(k + 1, 1e18));
    dp[0][0] = 0;
    for (auto m1 = 0U; m1 < 1 << k; m1++) {
        auto m2 = m1 ^ ((1 << k) - 1);
        for (int src = 0; src <= k; src++)
            if (dp[m1][src] != 1e18) {
                for (int i = 0; i < k; i++)
                    if ((m2 >> i) & 1)
                        if (d[src][i + 1] != 1e18) dp[m1 | (1 << i)][i + 1] = min(dp[m1 | (1 << i)][i + 1], dp[m1][src] + d[src][i + 1]);

                if (m1 == (1 << k) - 1)
                    if (d[src][0] != 1e18) dp[m1][0] = min(dp[m1][0], dp[m1][src] + d[src][0]);

                for (auto m3 = m2; m3; --m3 &= m2) {
                    if (cost2[src][m3] != 1e18) dp[m1 | m3][0] = min(dp[m1 | m3][0], dp[m1][src] + cost2[src][m3]);

                    for (auto m4 = m3; m4; m4 &= m4 - 1) {
                        int i = countr_zero(m4);
                        if (cost3[src][m3][i] != 1e18) dp[m1 | m3][i + 1] = min(dp[m1 | m3][i + 1], dp[m1][src] + cost3[src][m3][i]);
                    }
                }
            }
    }
    cout << dp.back()[0];
}
