#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> k(n + 1), root(n + 1);
    vector<unordered_set<int>> magic(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> k[i];

        if (!(1 <= i + k[i] && i + k[i] <= n)) magic[root[i] = i].emplace(k[i]);
    }

    vector<bool> visited(n + 1);
    for (int i = 1; i <= n; i++) {
        if (visited[i]) continue;

        int j = i;
        vector<int> hops;
        for (; 1 <= j && j <= n && !visited[j]; j += k[j]) {
            visited[j] = true;
            hops.emplace_back(j);
        }

        if (1 <= j && j <= n) {
            reverse(hops.begin(), hops.end());
            int len = find(hops.begin(), hops.end(), j) - hops.begin();
            if (len < hops.size())
                for (int l = 0; l <= len; l++) magic[root[hops[l]] = hops.front()].emplace(k[hops[l]]);
        }
    }

    vector<vector<int>> adj_list(n + 1);
    for (int v = 1; v <= n; v++)
        if (!root[v]) adj_list[v + k[v]].emplace_back(v);

    auto wins = 0LL;
    auto dfs = [&](auto &&self, int v, int r) -> void {
        bool inserted = magic[r].emplace(k[v]).second;
        wins += magic[r].size();
        for (int u : adj_list[v]) self(self, u, r);
        if (inserted) magic[r].erase(k[v]);
    };
    for (int v = 1; v <= n; v++)
        if (root[v]) dfs(dfs, v, root[v]);
    cout << wins;
}
