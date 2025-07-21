#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, t;
    cin >> n >> m >> t;

    vector<vector<array<int, 3>>> adj_list(n);
    while (m--) {
        int a, b, L, V;
        cin >> a >> b >> L >> V;
        a--;
        b--;

        adj_list[a].push_back({b, L, V});
        adj_list[b].push_back({a, L, V});
    }

    vector<double> dist(n);
    double l = 0, r = 1e9, mid;
    while (l + 1e-7 < r && l + l * 1e-7 < r) {
        mid = l + (r - l) / 2;

        auto valid = [&]() {
            fill(dist.begin(), dist.end(), 1e20);
            dist[0] = 0;
            priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
            pq.emplace(0, 0);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (d != dist[v]) continue;
                if (d > t || v == n - 1) break;

                for (auto [u, L, V] : adj_list[v])
                    if (dist[u] > d + L / (V + mid)) {
                        dist[u] = d + L / (V + mid);
                        pq.emplace(dist[u], u);
                    }
            }
            return dist[n - 1] <= t;
        };

        if (valid()) r = mid;
        else l = mid;
    }
    cout << fixed << setprecision(6) << l;
}
