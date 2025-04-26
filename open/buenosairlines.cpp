#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 4>> cities(n);
    for (int i = 0; i < n; i++) {
        int y, d, r;
        cin >> y >> d >> r;

        cities[i] = {y, d, r, i};
    }
    sort(cities.begin(), cities.end());

    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[cities[i][3]] = i;

    auto rev = cities;
    reverse(rev.begin(), rev.end());

    vector<vector<pair<int, int>>> adj_list(3 * n);
    for (int i = n; i < 2 * n; i++) {
        adj_list[i].emplace_back(i - n, 0);
        if (i + 1 < 2 * n) adj_list[i].emplace_back(i + 1, cities[i - n + 1][0] - cities[i - n][0]);
    }
    for (int i = 2 * n; i < 3 * n; i++) {
        adj_list[i].emplace_back(i - 2 * n, 0);
        if (i + 1 < 3 * n) adj_list[i + 1].emplace_back(i, cities[i - 2 * n + 1][0] - cities[i - 2 * n][0]);
    }

    for (int i = 0; i < n; i++) {
        auto it1 = lower_bound(cities.begin() + i + 1, cities.end(), cities[i][0] + cities[i][1], [](auto &c, int y) { return c[0] < y; });
        if (it1 != cities.end()) {
            int j = it1 - cities.begin();
            adj_list[i].emplace_back(j + n, cities[i][2] + cities[j][0] - cities[i][0]);
        }

        auto it2 = lower_bound(rev.begin() + n - i, rev.end(), cities[i][0] - cities[i][1], [](auto &c, int y) { return c[0] > y; });
        if (it2 != rev.end()) {
            int j = n - 1 - (it2 - rev.begin());
            adj_list[i].emplace_back(j + 2 * n, cities[i][2] + abs(cities[j][0] - cities[i][0]));
        }
    }

    vector<long long> dist(3 * n, 1e18);
    dist[indices[0]] = 0;
    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
    pq.emplace(0, indices[0]);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist[v]) continue;

        for (auto [u, w] : adj_list[v])
            if (dist[u] > dist[v] + w) {
                dist[u] = dist[v] + w;
                pq.emplace(dist[u], u);
            }
    }

    indices.erase(indices.begin());
    for (int i : indices) cout << (dist[i] == 1e18 ? -1 : dist[i]) << "\n";
}
