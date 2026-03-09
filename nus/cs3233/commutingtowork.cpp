#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, T;
    cin >> n >> m >> T;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int a, b, w;
        cin >> a >> b >> w;

        adj_list[a].emplace_back(b, w);
        adj_list[b].emplace_back(a, w);
    }

    vector<array<int, 3>> lights(n + 1, {0, 0, 0});
    for (int i = 2; i < n; i++) {
        int t, l;
        cin >> t >> l;

        lights[i][0] = t;
        if (t + l <= T) lights[i][1] = t + l;
        else {
            lights[i][1] = t + l - T;
            lights[i][2] = 1;
        }
    }

    vector<long long> times{0}, dist(n + 1);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int i = 2; i < n; i++) {
        auto [l, r, _] = lights[i];
        int tl = (l + T - 1) % T, tr = (r + T - 1) % T;

        auto dist_rev = [&](auto t) {
            fill(dist.begin(), dist.end(), 1e18);
            dist[i] = 0;
            pq.emplace(0, i);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (d != dist[v]) continue;

                for (auto [u, w] : adj_list[v]) {
                    if (dist[u] > d + w) {
                        int rem = (t - (d + w) % T + T) % T;
                        if (!lights[u][2]) {
                            if (lights[u][0] <= rem && rem < lights[u][1]) continue;
                        } else if (rem < lights[u][1] || rem >= lights[u][0]) continue;
                        dist[u] = d + w;
                        pq.emplace(d + w, u);
                    }
                }
            }
            return dist[1];
        };

        auto d1 = dist_rev(tl), d2 = dist_rev(tr);
        if (d1 != 1e18) times.emplace_back((tl - d1 % T + T) % T);
        if (d2 != 1e18) times.emplace_back((tr - d2 % T + T) % T);
    }
    sort(times.begin(), times.end());
    times.erase(unique(times.begin(), times.end()), times.end());

    auto f = LLONG_MAX;
    for (auto t : times) {
        fill(dist.begin(), dist.end(), 1e18);
        dist[1] = t;
        pq.emplace(t, 1);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist[v]) continue;

            if (v == n) {
                f = min(f, d - t);
                break;
            }

            int rem = d % T;
            if (!lights[v][2]) {
                if (lights[v][0] <= rem && rem < lights[v][1]) d += lights[v][1] - rem;
            } else {
                if (rem < lights[v][1]) d += lights[v][1] - rem;
                else if (rem >= lights[v][0]) d += lights[v][1] - rem + T;
            }

            for (auto [u, w] : adj_list[v])
                if (dist[u] > d + w) {
                    dist[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }
    }
    cout << f;
}