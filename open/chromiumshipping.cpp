#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX / 4;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t, a, b;
    cin >> n >> m >> s >> t >> a >> b;

    vector<int> x(s), y(t);
    for (int &xi : x) cin >> xi;
    for (int &yi : y) cin >> yi;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int u, v, d;
        cin >> u >> v >> d;

        adj_list[u].emplace_back(v, d);
        adj_list[v].emplace_back(u, d);
    }

    auto dijkstra = [&](int src) {
        vector<long long> dist(n + 1, 1e18);
        dist[src] = 0;
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d != dist[u]) continue;

            for (auto [v, w] : adj_list[u])
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.emplace(d + w, v);
                }
        }
        return dist;
    };
    auto dist_a = dijkstra(a), dist_b = dijkstra(b);

    vector<long long> e_a(s), e_b(s);
    for (int i = 0; i < s; i++) {
        e_a[i] = dist_a[x[i]];
        e_b[i] = dist_b[x[i]];
    }

    auto sum_c_b = 0LL;
    vector<long long> c_diff(t);
    for (int i = 0; i < t; i++) {
        sum_c_b += dist_b[y[i]];
        c_diff[i] = dist_a[y[i]] - dist_b[y[i]];
    }
    sort(c_diff.begin(), c_diff.end());

    vector<long long> pref(t + 1, 0);
    for (int k = 1; k <= t; k++) pref[k] = pref[k - 1] + c_diff[k - 1];

    vector<int> order(s);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return e_b[i] != e_b[j] ? e_b[i] < e_b[j] : i < j; });

    vector<bool> in_b(s, false), in_a(s, false);
    set<pair<long long, int>> pq_e_diff, pq_e_a, pq_e_b;
    auto sum_e_b = 0LL, sum_e_a = 0LL;
    for (int k = 0; k < t; k++) {
        int i = order[k];
        in_b[i] = true;
        sum_e_b += e_b[i];
        pq_e_diff.emplace(e_a[i] - e_b[i], i);
        pq_e_b.emplace(e_b[i], i);
    }

    for (int k = t; k < s; k++) {
        int i = order[k];
        pq_e_a.emplace(e_a[i], i);
    }

    vector<long long> sum_e(t + 1, 0);
    sum_e[0] = sum_e_b;
    for (int l = 1; l <= t; l++) {
        while (!pq_e_diff.empty() && !in_b[pq_e_diff.begin()->second]) pq_e_diff.erase(pq_e_diff.begin());
        while (!pq_e_a.empty() && (in_a[pq_e_a.begin()->second] || in_b[pq_e_a.begin()->second])) pq_e_a.erase(pq_e_a.begin());
        while (!pq_e_b.empty() && !in_b[pq_e_b.rbegin()->second]) pq_e_b.erase(prev(pq_e_b.end()));

        auto d_diff = LLONG_MAX, d_a = LLONG_MAX;
        int i = -1, j = -1;
        if (!pq_e_diff.empty()) tie(d_diff, i) = *pq_e_diff.begin();
        if (!pq_e_a.empty()) tie(d_a, j) = *pq_e_a.begin();

        if (d_diff <= d_a - (pq_e_b.empty() ? 0 : pq_e_b.rbegin()->first)) {
            pq_e_diff.erase(pq_e_diff.begin());
            pq_e_b.erase(pq_e_b.find({e_b[i], i}));
            sum_e_a += e_a[i];
            sum_e_b -= e_b[i];
            in_a[i] = true;
            in_b[i] = false;
        } else {
            pq_e_a.erase(pq_e_a.begin());
            sum_e_a += e_a[j];
            in_a[j] = true;

            auto it = prev(pq_e_b.end());
            auto [d_b, k] = *it;
            pq_e_b.erase(it);

            pq_e_diff.erase(pq_e_diff.find({e_a[k] - e_b[k], k}));
            sum_e_b -= d_b;
            in_b[k] = false;
            if (!in_a[k]) pq_e_a.emplace(e_a[k], k);
        }
        sum_e[l] = sum_e_a + sum_e_b;
    }

    auto d = LLONG_MAX;
    for (int k = 0; k <= t; k++) d = min(d, sum_e[k] + sum_c_b + pref[k]);
    cout << d;
}
