#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m) {
        vector<vector<int>> adj_list(n + 1);
        while (m--) {
            int x, y;
            cin >> x >> y;

            adj_list[x].emplace_back(y);
            adj_list[y].emplace_back(x);
        }

        vector<bool> visited(n + 1, false);
        auto dfs = [&](auto &&self, int v) -> int {
            visited[v] = true;
            if (adj_list[v].size() == 1) return 1;

            int count = 0;
            for (int u : adj_list[v])
                if (!visited[u]) count += self(self, u);

            return count;
        };
        for (int i = 1; i <= n; i++)
            if (!visited[i])
                if (dfs(dfs, i) >= 4) {
                    cout << "YES\n";
                    goto next;
                }
        cout << "NO\n";
        next:;
    }
}
