#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, c;
    cin >> n >> m >> c;

    vector<vector<array<int, 4>>> adj_list(n + 1);
    while (m--) {
        int a, b, t, l, r;
        cin >> a >> b >> t >> l >> r;

        adj_list[a].push_back({b, t, l, r});
        adj_list[b].push_back({a, t, l, r});
    }

    vector<long long> dist(n + 1, 1e18);
    dist[1] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (v == n) {
            cout << d;
            exit(0);
        }

        if (d > dist[v]) continue;

        for (auto [u, w, l, r] : adj_list[v]) {
            auto t = d + w;

            int remaining = d % c;
            if (l > remaining) t += l - remaining;
            else if (remaining > r) t += c + l - remaining;

            if (dist[u] > t) {
                dist[u] = t;
                pq.emplace(t, u);
            }
        }
    }
}
