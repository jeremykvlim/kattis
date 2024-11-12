#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(pair<array<int, 3>, array<int, 3>> p) const {
        auto h1 = 0ULL, h2 = 0ULL;
        for (int e : p.first) h1 ^= e + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        for (int e : p.second) h2 ^= e + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
        return h1 + h2;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    while (cin >> c && c) {
        unordered_map<pair<array<int, 3>, array<int, 3>>, vector<int>, Hash> connections;
        vector<vector<int>> adj_list(c + 1);
        for (int i = 1; i <= c; i++) {
            int n;
            cin >> n;

            vector<array<int, 3>> corners(n);
            for (auto &[x, y, z] : corners) cin >> x >> y >> z;

            for (int j = 0; j < n; j++) {
                auto prev = corners[j], curr = corners[(j + 1) % n];

                for (int k : connections[{min(prev, curr), max(prev, curr)}]) {
                    adj_list[i].emplace_back(k);
                    adj_list[k].emplace_back(i);
                }

                connections[{min(prev, curr), max(prev, curr)}].emplace_back(i);
            }
        }

        int m;
        cin >> m;

        while (m--) {
            int a, b;
            cin >> a >> b;

            vector<int> dist(c + 1, INT_MAX);
            dist[a] = 0;
            queue<int> q;
            q.emplace(a);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (dist[u] > dist[v] + 1) {
                        dist[u] = dist[v] + 1;
                        q.emplace(u);
                    }
            }

            cout << dist[b] << "\n";
        }
    }
}
