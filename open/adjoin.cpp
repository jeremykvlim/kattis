#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c, l;
    cin >> c >> l;

    vector<vector<int>> adj_list(c);
    while (l--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    vector<int> diameter, depth(c, -1);
    for (int i = 0; i < c; i++) {
        if (depth[i] != -1) continue;
        auto dfs = [&](auto &&self, int v, int prev = -1, int d = 0) -> int {
            int t = v;
            depth[v] = d;
            for (int u : adj_list[v])
                if (u != prev) {
                    int s = self(self, u, v, d + 1);
                    if (depth[t] < depth[s]) t = s;
                }
            return t;
        };
        diameter.emplace_back(depth[dfs(dfs, dfs(dfs, i))]);
    }
    sort(diameter.rbegin(), diameter.rend());

    int hops = diameter[0];
    if (diameter.size() >= 2) hops = max(hops, (diameter[0] + 1) / 2 + (diameter[1] + 1) / 2 + 1);
    if (diameter.size() >= 3) hops = max(hops, (diameter[1] + 1) / 2 + (diameter[2] + 1) / 2 + 2);
    cout << hops;
}
