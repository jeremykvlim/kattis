#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, m, s, t;
        cin >> n >> m >> s >> t;

        vector<vector<pair<int, int>>> adj_list(n + 1);
        while (m--) {
            int a, b, f;
            cin >> a >> b >> f;

            adj_list[a].emplace_back(b, f);
            adj_list[b].emplace_back(a, f);
        }

        vector<vector<int>> dist1(n + 1, vector<int>(n + 1, INT_MAX));
        vector<int> x(s), price(n + 1);
        for (int i = 0; i < s; i++) {
            cin >> x[i] >> price[x[i]];

            dist1[x[i]][x[i]] = 0;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            pq.emplace(0, x[i]);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (d != dist1[x[i]][v]) continue;

                for (auto [u, w] : adj_list[v])
                    if (dist1[x[i]][u] > d + w) {
                        dist1[x[i]][u] = d + w;
                        pq.emplace(d + w, u);
                    }
            }
        }

        int c, D;
        cin >> c >> D;

        x.emplace_back(D);
        vector<unordered_map<int, int>> dist2(n + 1);
        dist2[c][0] = 0;
        priority_queue<array<int, 3>, vector<array<int, 3>>, greater<>> pq;
        pq.push({0, c, 0});
        while (!pq.empty()) {
            auto [d, v, petrol] = pq.top();
            pq.pop();

            if (d != dist2[v][petrol]) continue;

            if (v == D) {
                cout << d << "\n";
                break;
            }

            for (int u : x) {
                int req = dist1[v][u];

                if (petrol <= req && req <= t) {
                    int w = (req - petrol) * price[v];

                    if (!dist2[u].count(0) || dist2[u][0] > d + w) {
                        dist2[u][0] = d + w;
                        pq.push({d + w, u, 0});
                    }
                }

                if (t > req) {
                    int w = (t - petrol) * price[v];

                    if (!dist2[u].count(t - req) || dist2[u][t - req] > d + w) {
                        dist2[u][t - req] = d + w;
                        pq.push({d + w, u, t - req});
                    }
                }
            }
        }
    }
}
