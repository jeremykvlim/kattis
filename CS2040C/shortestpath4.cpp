#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;

    while (tc--) {
        int v;
        cin >> v;

        vector<vector<pair<int, int>>> adj_list(v);
        for (int i = 0; i < v; i++) {
            int x;
            cin >> x;

            while (x--) {
                int u, w;
                cin >> u >> w;

                adj_list[i].emplace_back(u, w);
            }
        }

        int q;
        cin >> q;

        while (q--) {
            int s, t, k;
            cin >> s >> t >> k;

            if (s == t) {
                cout << "0\n";
                continue;
            }

            priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
            pq.emplace(0, s, 1);
            vector<vector<int>> dist(k + 1, vector<int>(v, INT_MAX));
            dist[1][s] = 0;
            while (!pq.empty()) {
                auto [d, v, junctions] = pq.top();
                pq.pop();

                if (v == t) {
                    cout << d << "\n";
                    goto next;
                }

                if (junctions >= k) continue;

                for (auto [u, w] : adj_list[v])
                    if (dist[junctions + 1][u] > dist[junctions][v] + w) {
                        dist[junctions + 1][u] = dist[junctions][v] + w;
                        pq.emplace(dist[junctions][v] + w, u, junctions + 1);
                    }
            }

            cout << "-1\n";
            next:;
        }

        cout << "\n";
    }
}
