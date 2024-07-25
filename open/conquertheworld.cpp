#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v, c;
        cin >> u >> v >> c;

        adj_list[u].emplace_back(v, c);
        adj_list[v].emplace_back(u, c);
    }

    int total = 0;
    vector<int> x(n + 1), y(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];

        int m =  min(x[i], y[i]);
        x[i] -= m;
        y[i] -= m;
        total += y[i];
    }

    auto sum = 0LL;
    vector<long long> dist(n + 1, 0);
    vector<__gnu_pbds::priority_queue<long long, greater<>, pairing_heap_tag>> pqx(n + 1), pqy(n + 1);
    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
        while (x[v]--) pqx[v].push(dist[v]);
        while (y[v]--) pqy[v].push(dist[v] - (long long) 1e12);

        for (auto [u, w] : adj_list[v])
            if (u != prev) {
                dist[u] = dist[v] + w;
                self(self, u, v);
                pqx[v].join(pqx[u]);
                pqy[v].join(pqy[u]);
            }

        while (!pqx[v].empty() && !pqy[v].empty()) {
            auto d1 = pqx[v].top(), d2 = pqy[v].top(), s = d1 + d2 - 2 * dist[v];
            if (s >= 0) break;

            sum += s;
            pqx[v].pop();
            pqy[v].pop();
            pqx[v].push(2 * dist[v] - d2);
            pqy[v].push(2 * dist[v] - d1);
        }
    };
    dfs(dfs);

    cout << sum + (long long) 1e12 * total;
}
