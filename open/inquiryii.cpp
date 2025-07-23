#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DisjointSets dsu(n + 1);
    vector<vector<int>> adj_list(n + 1);
    vector<pair<int, int>> extra;
    while (m--) {
        int a, b;
        cin >> a >> b;

        if (dsu.unite(a, b)) {
            adj_list[a].emplace_back(b);
            adj_list[b].emplace_back(a);
        } else extra.emplace_back(a, b);
    }

    int size = 0;
    vector<bool> forbidden(n + 1);
    for (int mask = 0; mask < 1 << extra.size(); mask++) {
        fill(forbidden.begin(), forbidden.end(), false);
        for (int i = 0; i < extra.size(); i++) {
            auto [a, b] = extra[i];
            forbidden[!((mask >> i) & 1) ? a : b] = true;
        }

        auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> pair<int, int> {
            int v0 = 0, v1 = forbidden[v] ? -1 : 1;
            for (int u : adj_list[v])
                if (u != prev) {
                    auto [u0, u1] = self(self, u, v);
                    v0 += max(u0, u1);
                    if (v1 > -1) v1 += u0;
                }
            return {v0, v1};
        };

        auto [dp0, dp1] = dfs(dfs);
        size = max({size, dp0, dp1});
    }
    cout << size;
}
