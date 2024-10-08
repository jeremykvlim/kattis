#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, a, k;
    while (cin >> n >> m >> a >> k) {
        vector<vector<pair<int, int>>> adj_list(n);
        vector<int> dist(n, k);
        while (m--) {
            int t1, t2, d;
            cin >> t1 >> t2 >> d;

            adj_list[t1 - 1].emplace_back(t2 - 1, d);
            adj_list[t2 - 1].emplace_back(t1 - 1, d);
        }

        while (a--) {
            int b;
            cin >> b;

            auto dfs = [&](auto &&self, int v, int d = 0) {
                if (dist[v] <= d) return;
                if (dist[v] == k) n--;
                dist[v] = d;
                for (auto [u, w] : adj_list[v]) self(self, u, d + w);
            };

            if (n) dfs(dfs, b - 1);
            cout << n << "\n";
        }

        cout << "\n";
    }
}
