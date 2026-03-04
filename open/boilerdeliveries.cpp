#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    for (auto &[u, v, p] : edges) {
        cin >> u >> v >> p;
        u--;
        v--;
        p = -p;
    }

    vector<long long> potential(n, 0);
    for (int _ = 0; _ < n; _++) {
        bool changed = false;
        for (auto [u, v, w] : edges) {
            if (potential[v] > potential[u] + w) {
                potential[v] = potential[u] + w;
                changed = true;
            }
        }
        if (!changed) break;
    }

    vector<vector<pair<int, int>>> adj_list(n);
    for (auto [u, v, w] : edges) adj_list[u].emplace_back(v, w + potential[u] - potential[v]);

    vector<int> dist1(n);
    vector<vector<int>> dist2(n, vector<int>(n, 1e9));
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    for (int s = 0; s < n; s++) {
        fill(dist1.begin(), dist1.end(), 1e9);
        dist1[s] = 0;
        pq.emplace(0, s);

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

        for (int v = 0; v < n; v++)
            if (dist1[v] != 1e9) dist2[s][v] = dist1[v] - potential[s] + potential[v];
    }

    int q;
    cin >> q;

    while (q--) {
        int s, t, k;
        cin >> s >> t >> k;
        s--;
        t--;

        bool path = false;
        int value = 0;
        while (k--) {
            int v;
            cin >> v;
            v--;

            int a = dist2[s][v], b = dist2[v][t];
            if (a == 1e9 || b == 1e9) continue;
            if (!path || value > a + b) {
                path = true;
                value = a + b;
            }
        }

        if (!path) cout << "NO PATH\n";
        else cout << -value << "\n";
    }
}
