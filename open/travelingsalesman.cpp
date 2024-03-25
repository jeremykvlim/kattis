#include <bits/stdc++.h>
using namespace std;

struct Hash {
    uint64_t encode(const tuple<int, int, int> &t) const {
        uint64_t encoded = 0;
        encoded = (encoded << 8) | get<0>(t);
        encoded = (encoded << 8) | get<1>(t);
        encoded = (encoded << 8) | get<2>(t);
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        uint64_t hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const pair<tuple<int, int, int>, tuple<int, int, int>> &p) const {
        static const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(p.first) + encode(p.second) + SEED);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    while (cin >> c && c) {
        unordered_map<pair<tuple<int, int, int>, tuple<int, int, int>>, vector<int>, Hash> connections;
        vector<vector<int>> adj_list(c + 1);
        for (int i = 1; i <= c; i++) {
            int n;
            cin >> n;

            vector<tuple<int, int, int>> corners(n);
            for (auto &t : corners) cin >> get<0>(t) >> get<1>(t) >> get<2>(t);

            for (int j = 0; j < n; j++) {
                auto &prev = corners[j], &curr = corners[(j + 1) % n];

                for (auto &k : connections[{min(prev, curr), max(prev, curr)}]) {
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

                for (auto u : adj_list[v])
                    if (dist[u] > dist[v] + 1) {
                        dist[u] = dist[v] + 1;
                        q.emplace(u);
                    }
            }

            cout << dist[b] << "\n";
        }
    }
}
