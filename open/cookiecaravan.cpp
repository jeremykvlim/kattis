#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n, m;
    cin >> k >> n >> m;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int a, b, c;
        cin >> a >> b >> c;

        adj_list[a].emplace_back(b, c);
        adj_list[b].emplace_back(a, c);
    }

    vector<vector<long long>> dist(n + 1, vector<long long>(k, 1e18));
    dist[1][0] = 0;
    priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>, greater<tuple<long long, int, int>>> pq;
    pq.emplace(0, 1, 0);
    while (!pq.empty()) {
        auto [d, v, r] = pq.top();
        pq.pop();

        if (d != dist[v][r]) continue;

        for (auto [u, c] : adj_list[v]) {
            int rem = (r + c) % k;
            if (dist[u][rem] > d + c) {
                dist[u][rem] = d + c;
                pq.emplace(dist[u][rem], u, rem);
            }
        }
    }

    pair<int, long long> least{k + 1, 1e18};
    for (int r = 0; r < k; r++)
        if (dist[n][r] != 1e18) {
            int rem = (k - r) % k;
            least = min(least, make_pair(rem, dist[n][r]));
        }
    cout << least.second;
}
