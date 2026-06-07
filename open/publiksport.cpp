#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> adj_list(n + 1);
    vector<int> degree(n + 1, 0);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
        degree[a]++;
        degree[b]++;
    }

    if (k == 1) {
        cout << *max_element(degree.begin(), degree.end());
        exit(0);
    }

    int sum = -1e9;
    vector<int> depth(n + 1, 0), pref(n + 1, 0);
    pref[1] = degree[1];
    auto dfs = [&](auto &&self, int v = 1, int prev = 0) -> unordered_map<int, int> {
        vector<unordered_map<int, int>> maps;
        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                pref[u] = pref[v] + degree[u];
                maps.emplace_back(self(self, u, v));
            }
        if (maps.empty()) return {{depth[v], pref[v]}};

        int j = 0;
        for (int i = 1; i < maps.size(); i++)
            if (maps[j].size() < maps[i].size()) j = i;

        auto m = move(maps[j]);
        auto it = m.find(depth[v] + k - 1);
        if (it != m.end()) sum = max(sum, it->second - pref[v] + degree[v]);

        auto [it1, inserted1] = m.emplace(depth[v], pref[v]);
        if (!inserted1) it1->second = max(it1->second, pref[v]);

        for (int i = 0; i < maps.size(); i++)
            if (i != j) {
                for (auto [d, p] : maps[i]) {
                    it = m.find(2 * depth[v] + k - 1 - d);
                    if (it != m.end()) sum = max(sum, p + it->second - 2 * pref[v] + degree[v]);
                }

                for (auto [d, p] : maps[i]) {
                    auto [it2, inserted2] = m.emplace(d, p);
                    if (!inserted2) it2->second = max(it2->second, p);
                }
            }
        return m;
    };
    dfs(dfs);

    if (sum == -1e9) cout << "-";
    else cout << sum - 2 * (k - 1);
}
