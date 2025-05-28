#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, m, n;
    cin >> d >> m >> n;

    vector<vector<int>> adj_list(d + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[b].emplace_back(a);
    }

    vector<bool> visited(d + 1, false);
    vector<bitset<1001>> bs(d + 1);
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        if (adj_list[v].empty()) bs[v][v] = true;
        for (int u : adj_list[v]) {
            if (!visited[u]) self(self, u);
            bs[v] |= bs[u];
        }
    };
    for (int i = 1; i <= d; i++)
        if (!visited[i]) dfs(dfs, i);

    vector<int> events(n);
    for (int &x : events) cin >> x;

    for (int i = 1; i <= d; i++)
        for (int x : events)
            if ((bs[x] & ~bs[i]).none()) {
                cout << i << " ";
                break;
            }
}
