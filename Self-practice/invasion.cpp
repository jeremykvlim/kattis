#include <bits/stdc++.h>
using namespace std;

void dfs(int v, int d, int &k, int &n, vector<int> &dist, vector<vector<pair<int, int>>> &adj_list) {
    if (dist[v] <= d) return;
    if (dist[v] == k) n--;
    dist[v] = d;
    for (auto u : adj_list[v]) dfs(u.first, d + u.second, k, n, dist, adj_list);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, a, k;
    while (cin >> n >> m >> a >> k) {
        vector<vector<pair<int, int>>> adj_list(n);
        vector<int> dist(n, k);
        for (int i = 0; i < m; i++) {
            int t1, t2, d;
            cin >> t1 >> t2 >> d;
            
            adj_list[t1 - 1].emplace_back(t2 - 1, d);
            adj_list[t2 - 1].emplace_back(t1 - 1, d);
        }

        for (int i = 0; i < a; i++) {
            int b;
            cin >> b;
            
            if (n) dfs(b - 1, 0, k, n, dist, adj_list);
            cout << n << "\n";
        }
        
        cout << "\n";
    }
}
