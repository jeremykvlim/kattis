#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    double b, s, r;
    cin >> n >> m >> b >> s >> r;

    vector<vector<pair<int, double>>> adj_list(n + 1);
    while (m--) {
        int x, y;
        double p;
        cin >> x >> y >> p;

        adj_list[x].emplace_back(y, p);
    }

    vector<double> dist1(n + 1, 1e20), dist2(n + 1);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq1, pq2;
    dist1[1] = 0;
    pq1.emplace(0, 1);
    auto dijkstra = [&](int src) {
        fill(dist2.begin(), dist2.end(), 1e20);
        dist2[src] = r;
        pq2.emplace(r, src);
        while (!pq2.empty()) {
            auto [dv, v] = pq2.top();
            pq2.pop();

            if (dv != dist2[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (w != 0) {
                    auto du = (dv + s) / w;
                    if (dist2[u] > du) {
                        dist2[u] = du;
                        pq2.emplace(du, u);
                    }
                }
        }
    };

    while (!pq1.empty()) {
        auto [dv, v] = pq1.top();
        pq1.pop();

        if (dv != dist1[v]) continue;

        if (v == n) {
            cout << fixed << setprecision(5) << dv << "\n";
            exit(0);
        }

        dijkstra(v);

        for (int u = 1; u <= n; u++)
            if (u != v && dist2[u] != 1e20) {
                auto du = dv + dist2[u] - r + (u == n ? 0 : b);
                if (dist1[u] > du) {
                    dist1[u] = du;
                    pq1.emplace(du, u);
                }
            }
    }
}
