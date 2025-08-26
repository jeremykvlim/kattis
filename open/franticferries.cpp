#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, j, r, f, jf, ji;
    cin >> T >> j >> r >> f >> jf >> ji;

    vector<vector<pair<int, int>>> roads(j);
    while (r--) {
        int j1, j2, t;
        cin >> j1 >> j2 >> t;

        roads[j1].emplace_back(j2, t);
        roads[j2].emplace_back(j1, t);
    }

    vector<vector<array<int, 4>>> ferries(j);
    while (f--) {
        int j1, j2, d, tout, tin;
        cin >> j1 >> j2 >> d >> tout >> tin;

        ferries[j1].push_back({j2, tin, d + tout, tout + tin});
        ferries[j2].push_back({j1, tout, d, tout + tin});
    }

    vector<long long> dist(j, 1e18);
    dist[ji] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, ji);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist[v]) continue;

        for (auto [u, w] : roads[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(dist[u], u);
            }

        for (auto [u, w, first, t] : ferries[v])
            if (T - d - w >= first) {
                auto k = (T - d - w - first) % t;
                if (dist[u] > d + w + k) {
                    dist[u] = d + w + k;
                    pq.emplace(dist[u], u);
                }
            }
    }
    cout << T - dist[jf];
}
