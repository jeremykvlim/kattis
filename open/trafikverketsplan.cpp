#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<pair<int, bool>>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b, true);
        adj_list[b].emplace_back(a, false);
    }

    int lg = __lg(n) + 1;
    vector<vector<int>> lift(lg, vector<int>(n + 1, 0)), OR_lift(lg, vector<int>(n + 1, 0));
    vector<int> up(n + 1, 0), down(n + 1, 0), depth(n + 1, 0);
    vector<bool> visited(n + 1, false);
    visited[1] = true;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, d] : adj_list[v]) {
            if (!visited[u]) {
                visited[u] = true;
                lift[0][u] = v;
                depth[u] = depth[v] + 1;
                if (d) {
                    down[u] = down[v] + 1;
                    up[u] = up[v];
                } else {
                    up[u] = up[v] + 1;
                    down[u] = down[v];
                }
                q.emplace(u);
            }
        }
    }

    for (int i = 1; i < lg; i++)
        for (int v = 1; v <= n; v++) lift[i][v] = lift[i - 1][lift[i - 1][v]];

    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);

        int diff = depth[u] - depth[v];
        for (int i = 0; i < lg; i++)
            if ((diff >> i) & 1) u = lift[i][u];

        if (u == v) return u;

        for (int i = lg - 1; ~i; i--)
            if (lift[i][u] != lift[i][v]) {
                u = lift[i][u];
                v = lift[i][v];
            }

        return lift[0][u];
    };

    int Q;
    cin >> Q;

    while (Q--) {
        int h, w;
        cin >> h >> w;
        
        int a = lca(h, w);
        if (up[h] - up[a] != depth[h] - depth[a] || down[w] - down[a] != depth[w] - depth[a]) {
            cout << "nej\n";
            continue;
        }
        cout << "ja\n";
    }
}
