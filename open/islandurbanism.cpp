#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<vector<T>> dreyfus_wagner(int n, const vector<array<int, 3>> &edges, const vector<int> &terminals) {
    vector<vector<pair<int, int>>> adj_list(n);
    for (auto [u, v, w] : edges) adj_list[u].emplace_back(v, w);

    int t = terminals.size();
    T inf = numeric_limits<T>::max() >> 2;
    vector<vector<T>> dp(1 << t, vector<T>(n, inf));
    for (int i = 0; i < t; i++) dp[1 << i][terminals[i]] = 0;

    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> pq;
    for (int m1 = 1; m1 < 1 << t; m1++) {
        for (int m2 = (m1 - 1) & m1; m2; --m2 &= m1)
            if ((m1 ^ m2) >= m2)
                for (int v = 0; v < n; v++) dp[m1][v] = min(dp[m1][v], dp[m2][v] + dp[m1 ^ m2][v]);

        auto &dist = dp[m1];
        for (int v = 0; v < n; v++)
            if (dist[v] != inf) pq.emplace(dist[v], v);

        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, V, k;
    cin >> n >> m >> V >> k;

    vector<int> u(V), offset(V + 1, 0);
    for (int i = 0; i < V; i++) {
        cin >> u[i];

        offset[i + 1] = offset[i] + u[i];
    }

    vector<int> village(n);
    for (int i = 0; i < V; i++)
        for (int j = offset[i]; j < offset[i + 1]; j++) village[j] = i;

    vector<int> replacement(V);
    vector<vector<array<int, 3>>> edges(V);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;

        int va = village[a], vb = village[b];
        if (va == vb) {
            edges[va].push_back({a - offset[va], b - offset[va], c});
            edges[va].push_back({b - offset[va], a - offset[va], c});
        } else if ((va + 1) % V == vb) replacement[va] = c;
        else replacement[vb] = c;
    }

    vector<vector<int>> terminals(V);
    for (int i = 0; i < k; i++) {
        int t;
        cin >> t;
        t--;

        terminals[village[t]].emplace_back(t - offset[village[t]]);
    }

    vector<int> active;
    vector<long long> base(V), left(V), right(V), both(V), split(V, LLONG_MAX);
    for (int i = 0; i < V; i++) {
        int s = terminals[i].size();
        if (s) active.emplace_back(i);
        terminals[i].emplace_back(0);
        terminals[i].emplace_back(u[i] - 1);
        auto dp = dreyfus_wagner<long long>(u[i], edges[i], terminals[i]);

        int dest = (1 << s) - 1, l = 1 << s, r = 1 << (s + 1);
        base[i] = dp[dest][terminals[i][0]];
        left[i] = dp[dest | l][0];
        right[i] = dp[dest | r][u[i] - 1];
        both[i] = dp[dest | l | r][0];
        split[i] = dp[l][0] + dp[dest | r][u[i] - 1];
        for (int mask = 1; mask <= dest; mask++) split[i] = min(split[i], dp[mask | l][0] + dp[(dest ^ mask) | r][u[i] - 1]);
    }

    vector<long long> pref1(2 * V + 1, 0), pref2(2 * V + 1, 0);
    for (int i = 0; i < 2 * V; i++) {
        pref1[i + 1] = pref1[i] + both[i % V];
        pref2[i + 1] = pref2[i] + replacement[i % V];
    }

    auto cost = LLONG_MAX;
    for (int i = 0; i < V; i++) cost = min(cost, pref1[V] + pref2[V] - both[i] + split[i]);
    for (int i = 0; i < active.size(); i++) {
        int l = active[i], r = active[(i + active.size() - 1) % active.size()];
        if (l > r) r += V;

        cost = min(cost, l == r ? base[l] : right[l] + left[r % V] + pref1[r] - pref1[l + 1] + pref2[r] - pref2[l]);
    }
    cout << cost;
}