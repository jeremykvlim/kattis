#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, f;
    cin >> n >> f;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int v, t;
        cin >> v >> t;

        adj_list[v].emplace_back(i, t);
        adj_list[i].emplace_back(v, t);
    }

    int l = -1, r = 1e9, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        vector<int> remaining(n + 1, -1), coverage(n + 1, 0);
        vector<bool> stationed(n + 1, false);
        stationed[1] = true;
        auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> void {
            for (auto [u, w] : adj_list[v]) {
                if (u == prev || stationed[u]) continue;

                self(self, u, v);
                remaining[v] = max(remaining[v], remaining[u] - w);
                if (coverage[u] == -1) continue;

                if (coverage[u] <= mid - w) coverage[v] = max(coverage[v], coverage[u] + w);
                else {
                    stationed[u] = true;
                    remaining[v] = max(remaining[v], mid - w);
                }
            }

            if (remaining[v] >= coverage[v]) {
                stationed[v] = false;
                coverage[v] = -1;
            }
        };
        dfs(dfs);

        if (count(stationed.begin(), stationed.end(), true) > f) l = mid;
        else r = mid;
    }

    cout << (l == -1 ? 0 : r);
}
