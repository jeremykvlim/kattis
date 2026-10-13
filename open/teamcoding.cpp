#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> l(n);
    for (int &li : l) cin >> li;

    vector<vector<int>> adj_list(n);
    for (int i = 1; i < n; i++) {
        int b;
        cin >> b;

        adj_list[b].emplace_back(i);
    }

    vector<int> depth(n, 0);
    vector<vector<pair<int, int>>> freq(k);
    auto dfs1 = [&](auto &&self, int v = 0) -> void {
        freq[l[v]].emplace_back(depth[v], 1);
        for (int u : adj_list[v]) {
            depth[u] = depth[v] + 1;
            self(self, u);
        }
    };
    dfs1(dfs1);

    for (auto &f : freq) {
        sort(f.begin(), f.end());
        int i = 0;
        for (auto [d, _] : f)
            if (i && f[i - 1].first == d) f[i - 1].second++;
            else f[i++] = {d, 1};
        f.resize(i);
    }

    int employees = 1, switches = 0;
    vector<unordered_map<int, int>> count(n);
    vector<int> deepest(n), nearest(k, -1), same(n, 1);
    auto dfs2 = [&](auto &&self, int v = 0) -> void {
        int t = exchange(nearest[l[v]], v);
        deepest[v] = depth[v];
        for (int u : adj_list[v]) {
            self(self, u);
            deepest[v] = max(deepest[v], deepest[u]);
            if (count[v].size() < count[u].size()) count[v].swap(count[u]);
            for (auto [d, f] : count[u]) count[v][d] += f;
        }

        count[v][depth[v]]++;
        if (~t) same[t] += same[v];
        else {
            int e = 0;
            for (auto it = lower_bound(freq[l[v]].begin(), freq[l[v]].end(), make_pair(depth[v], 0)); it != freq[l[v]].end() && it->first <= deepest[v]; it++) e += min(count[v][it->first], it->second);
            if (employees < e || employees == e && switches > e - same[v]) {
                employees = e;
                switches = e - same[v];
            }
        }
        nearest[l[v]] = t;
    };
    dfs2(dfs2);
    cout << employees << " " << switches;
}
