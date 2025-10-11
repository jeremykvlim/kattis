#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    double T;
    cin >> n >> m >> T;

    vector<vector<array<int, 5>>> adj_list(n + 1);
    while (m--) {
        int u, v, l, g, r, t;
        cin >> u >> v >> l >> g >> r >> t;

        adj_list[u].push_back({v, l, g, r, t});
    }

    vector<double> dist(n + 1);
    auto feasible = [&](double speed) -> bool {
        fill(dist.begin(), dist.end(), 1e20);
        dist[1] = 0;
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
        pq.emplace(0, 1);
        while (!pq.empty()) {
            auto [dv, v] = pq.top();
            pq.pop();

            if (dv != dist[v]) continue;

            if (v == n) break;

            for (auto [u, l, g, r, t] : adj_list[v]) {
                int c = g + r;
                auto w = l / speed;
                if (t - dv > 1e-6) w += t - dv;
                else {
                    auto time = dv - t - floor((dv - t) / c) * c;
                    if (time >= g) w += c - time;
                }

                if (dist[u] > dv + w + 1e-6 && T > dv + w + 1e-6) {
                    dist[u] = dv + w;
                    pq.emplace(dv + w, u);
                }
            }
        }
        return dist[n] <= T + 1e-6;
    };

    double l = 0, r = 1, mid;
    while (!feasible(r)) r *= 2;
    while (l + 1e-7 < r && l + l * 1e-7 < r) {
        mid = l + (r - l) / 2;

        if (feasible(mid)) r = mid;
        else l = mid;
    }
    cout << fixed << setprecision(6) << r;
}
