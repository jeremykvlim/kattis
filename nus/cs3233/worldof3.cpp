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

template <typename T, typename F>
void fast_subset_transform(int n, vector<T> &v, F &&f) {
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) v[i + j + k] = f(v[i + j + k], v[i + j]);
}

template <typename T>
T gospers_hack(T mask) {
    T lsb_mask = mask | (mask - 1);
    return (lsb_mask + 1) | (((~lsb_mask & -~lsb_mask) - 1) >> (countr_zero(mask) + 1));
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
                    pq.emplace(dv + w, u);
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

    vector<int> popcount(1 << k, 0);
    vector<vector<unsigned>> masks_by_pop(k + 1);
    masks_by_pop[0] = {0};
    for (auto mask = 1U; mask < 1 << k; mask++) masks_by_pop[popcount[mask] = popcount[mask & (mask - 1)] + 1].emplace_back(mask);

    vector<long long> cost(1 << k), temp(1 << k);
    vector<vector<long long>> dp1(k + 1, vector<long long>(1 << k, 1e18)), dp2(k + 1, vector<long long>(1 << k));
    dp1[0][0] = 0;
    for (int src = 0; src <= k; src++) {
        for (int p = 0; p <= k; p++)
            for (auto mask = 0U; mask < 1 << k; mask++) cost[mask] = dp2[p][mask] = 1e18;

        auto [tour, len, dp_tsp] = held_karp(k, d, src);
        for (auto m1 = 1U; m1 < 1 << k; m1++) {
            auto m2 = m1 & spell_mask;
            if (m2) {
                for (; m2; m2 &= m2 - 1) cost[m1] = min(cost[m1], dp_tsp[m1][countr_zero(m2)]);
                cost[m1] = max(cost[m1], (long long) t);
            }
        }

        for (int p = 1; p <= k; p++) {
            fill(temp.begin(), temp.end(), 1e18);
            for (auto mask : masks_by_pop[p]) temp[mask] = cost[mask];
            fast_subset_transform(1 << k, temp, [](auto x, auto y) { return min(x, y); });
            for (int q = 1; q <= p; q++)
                for (auto mask = 0U; mask < 1 << k; mask++) dp2[q][mask] = min(dp2[q][mask], temp[mask]);
        }

        bool src_spell = src && (spell_mask >> (src - 1)) & 1;
        for (int p = 0; p <= k; p++)
            for (auto m1 : masks_by_pop[p])
                if (dp1[src][m1] != 1e18) {
                    auto m2 = m1 ^ ((1 << k) - 1);

                    for (auto m3 = m2; m3; m3 &= m3 - 1) {
                        int i = countr_zero(m3);
                        if (d[src][i + 1] != 1e18) dp1[i + 1][m1 | (1 << i)] = min(dp1[i + 1][m1 | (1 << i)], dp1[src][m1] + d[src][i + 1]);
                    }

                    if (m1 == (1 << k) - 1)
                        if (d[src][0] < 1e18) dp1[0][m1] = min(dp1[0][m1], dp1[src][m1] + d[src][0]);

                    if (p == k) continue;

                    vector<int> pos;
                    for (auto m3 = m2; m3; m3 &= m3 - 1) pos.emplace_back(countr_zero(m3));
                    int m = pos.size();

                    for (int a = 1; a <= m; a++)
                        for (auto m3 = (1U << a) - 1; m3 && m3 < 1 << m; m3 = gospers_hack(m3)) {
                            auto m4 = 0U;
                            for (auto m5 = m3; m5; m5 &= m5 - 1) m4 |= (1 << pos[countr_zero(m5)]);

                            int c1 = max(1, a - a_max + 1);
                            if (src_spell && a < a_max) dp2[c1][m4] = min(dp2[c1][m4], (long long) t);
                            if (dp2[c1][m4] != 1e18) dp1[0][m1 | m4] = min(dp1[0][m1 | m4], dp1[src][m1] + dp2[c1][m4]);

                            int c2 = max(1, a - a_max);
                            if (c2 <= k)
                                for (auto m5 = m4; m5; m5 &= m5 - 1) {
                                    int i = countr_zero(m5), m6 = m4 ^ (1 << i);
                                    if (src_spell && a <= a_max) dp2[c2][m6] = min(dp2[c2][m6], (long long) t);
                                    if (dp2[c2][m6] != 1e18) dp1[i + 1][m1 | m4] = min(dp1[i + 1][m1 | m4], dp1[src][m1] + dp2[c2][m6]);
                                }
                        }
                }
    }
    cout << dp1[0].back();
}
