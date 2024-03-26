#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d, c;
    cin >> n >> d >> c;

    vector<int> infected(c);
    for (auto &i : infected) cin >> i;

    vector<tuple<int, int, int>> times(n);
    for (int i = 0; i < n; i++) {
        int s, t;
        cin >> s >> t;

        times[i] = {s, t, i};
    }
    sort(times.begin(), times.end());

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n; i++) {
        auto [si, ti, v] = times[i];
        for (int j = i + 1; j < n; j++) {
            auto [sj, tj, u] = times[j];

            if (ti < sj) break;

            adj_list[v].emplace_back(u);
            adj_list[u].emplace_back(v);
        }
    }

    queue<pair<int, int>> q;
    vector<int> dist(n, -1);
    for (int i : infected) q.emplace(i - 1, 0);
    while (!q.empty()) {
        auto [v, w] = q.front();
        q.pop();

        if (dist[v] >= 0) continue;

        dist[v] = w;
        for (int u : adj_list[v])
            if (dist[u] == -1) q.emplace(u, w + 1);
    }

    for (int i = 0; i < n; i++)
        if (0 <= dist[i] && dist[i] <= d) cout << i + 1 << " ";
}
