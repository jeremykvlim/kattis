#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < m; _++) {
        int u, v;
        cin >> u >> v;
        
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    int c0 = 0, c1 = 0;
    vector<int> color(n + 1, -1);
    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (!~color[i]) {
            color[i] = 0;
            c0++;
            q.emplace(i);
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                
                for (int u : adj_list[v]) {
                    if (!~color[u]) {
                        color[u] = color[v] ^ 1;
                        (!color[u] ? c0 : c1)++;
                        q.emplace(u);
                    }
                }
            }
        }

    cout << n / 2 - (!(n & 1) && (c0 & 1) && (c1 & 1) && (m == c0 * c1));
}
