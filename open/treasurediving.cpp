#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<vector<pair<int, int>>> adj_list(n);
        while (m--) {
            int a, b, l;
            cin >> a >> b >> l;

            adj_list[a].emplace_back(b, l);
            adj_list[b].emplace_back(a, l);
        }

        int i;
        cin >> i;

        vector<int> p(i);
        for (int &pi : p) cin >> pi;
        p.emplace_back(0);
        sort(p.begin(), p.end());

        int a;
        cin >> a;

        vector<vector<int>> dist_p(i + 1, vector<int>(i + 1));
        auto dijkstra = [&](int j) {
            vector<int> dist(n, 1e9);
            dist[p[j]] = 0;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            pq.emplace(0, p[j]);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (dist[v] < d) continue;

                for (auto [u, w] : adj_list[v]) {
                    if (d + w > a) continue;

                    if (dist[u] > d + w) {
                        dist[u] = d + w;
                        pq.emplace(dist[u], u);
                    }
                }
            }

            for (int k = 0; k <= i; k++) dist_p[j][k] = dist[p[k]];
        };

        vector<int> perm;
        for (int j = 0; j <= i; j++) {
            dijkstra(j);
            perm.emplace_back(j);
        }

        int x = 0;
        do {
            auto sum = 0LL;
            for (int j = 0; j < i; j++) {
                sum += dist_p[perm[j]][perm[j + 1]];
                if (sum + dist_p[perm[j + 1]][0] <= a) x = max(x, j + 1);
            }
        } while (next_permutation(perm.begin() + 1, perm.end()));
        cout << x << "\n";
    }
}
