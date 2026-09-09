#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, K, a, b;
    cin >> n >> m >> K >> a >> b;

    vector<vector<pair<int, int>>> adj_list(n);
    vector<array<int, 3>> negative;

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;

        if (w < 0) negative.push_back({u, v, w});
        else adj_list[u].emplace_back(v, w);
    }
    K = negative.size();

    vector<int> src(K + 1);
    src[0] = a;
    for (int i = 0; i < K; i++) src[i + 1] = negative[i][1];

    vector<long long> dist1(n);
    vector<vector<long long>> dist2(K + 2, vector<long long>(K + 2, 1e18));
    for (int i = 0; i < K + 2; i++) dist2[i][i] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int i = 0; i <= K; i++) {
        fill(dist1.begin(), dist1.end(), 1e18);
        dist1[src[i]] = 0;
        pq.emplace(0, src[i]);
        while (!pq.empty()) {
            auto [d, v] = pq.top();
            pq.pop();

            if (d != dist1[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist1[u] > d + w) {
                    dist1[u] = d + w;
                    pq.emplace(d + w, u);
                }
        }

        dist2[i][K + 1] = dist1[b];
        for (int j = 0; j < K; j++) {
            auto [u, v, w] = negative[j];
            if (dist1[u] != 1e18) dist2[i][j + 1] = min(dist2[i][j + 1], dist1[u] + w);
        }
    }

    for (int i = 0; i < K + 2; i++)
        for (int j = 0; j < K + 2; j++)
            for (int k = 0; k < K + 2; k++) dist2[j][k] = min(dist2[j][k], dist2[j][i] + dist2[i][k]);

    for (int i = 0; i < K + 2; i++)
        if (dist2[0][i] != 1e18 && dist2[i][i] < 0 && dist2[i][K + 1] != 1e18) {
            cout << "NEGATIVE INFINITY";
            exit(0);
        }

    if (dist2[0][K + 1] == 1e18) cout << "POSITIVE INFINITY";
    else cout << dist2[0][K + 1];
}