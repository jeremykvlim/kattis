#include <bits/stdc++.h>
using namespace std;

void dfs(int v, int d, int k, int &n, vector<int> &dist, vector<vector<pair<int, int>>> &adj_list) {
    if (dist[v] <= d) return;
    if (dist[v] == k) n--;
    dist[v] = d;
    for (auto [t, w] : adj_list[v]) dfs(t, d + w, k, n, dist, adj_list);
}

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

            if (n) dfs(b - 1, 0, k, n, dist, adj_list);
            cout << n << "\n";
        }

        cout << "\n";
    }
}
