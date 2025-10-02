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
        for (int v = 0; v < n; v++)
            for (int m2 = (m1 - 1) & m1; m2; --m2 &= m1) dp[m1][v] = min(dp[m1][v], dp[m2][v] + dp[m1 ^ m2][v]);

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

    int n, m;
    cin >> n >> m;

    unordered_map<string, int> cities;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        cities[s] = i;
    }

    vector<array<int, 3>> edges(2 * m);
    for (int i = 0; i < m; i++) {
        string u, v;
        int c;
        cin >> u >> v >> c;

        edges[2 * i] = {cities[u], cities[v], c};
        edges[2 * i + 1] = {cities[v], cities[u], c};
    }

    vector<int> terminals(8);
    for (int &terminal : terminals) {
        string s;
        cin >> s;

        terminal = cities[s];
    }
    auto dp = dreyfus_wagner<int>(n, edges, terminals);

    vector<int> cost(1 << 4);
    for (int m1 = 1; m1 < 1 << 4; m1++) {
        int m0 = 0;
        for (int i = 0; i < 4; i++)
            if ((m1 >> i) & 1) m0 |= (1 << (2 * i)) | (1 << (2 * i + 1));

        cost[m1] = dp[m0][terminals[__lg(m1 & -m1) << 1]];
        for (int m2 = m1; m2; --m2 &= m1) cost[m1] = min(cost[m1], cost[m2] + cost[m1 ^ m2]);
    }
    cout << cost.back();
}
