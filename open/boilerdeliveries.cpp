#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n);
    for (auto &[u, v, p] : edges) {
        cin >> u >> v >> p;
        u--;
        v--;
        p = -p;
        
        adj_list[u].emplace_back(v, p);
    }

    vector<long long> potential(n, 0);
    vector<bool> queued(n, true);
    deque<int> dq;
    for (int i = 0; i < n; i++) dq.emplace_back(i);

    while (!dq.empty()) {
        int v = dq.front();
        dq.pop_front();
        
        queued[v] = false;

        for (auto [u, w] : adj_list[v])
            if (potential[u] > potential[v] + w) {
                potential[u] = potential[v] + w;

                if (!queued[u]) {
                    queued[u] = true;
                    if (dq.empty() || potential[u] < potential[dq.front()]) dq.emplace_front(u);
                    else dq.emplace_back(u);
                }
            }
    }

    for (int v = 0; v < n; v++)
        for (auto &[u, w] : adj_list[v]) w += potential[v] - potential[u];

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