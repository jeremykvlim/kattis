#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> k(n + 1);
    vector<vector<int>> adj_list(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> k[i];

        int f;
        cin >> f;

        while (f--) {
            int j;
            cin >> j;

            adj_list[i].emplace_back(j);
        }
    }

    auto time = 0LL;
    vector<int> size(n + 1, 1);
    auto dfs = [&](auto &&self, int v = 1) -> void {
        vector<pair<long long, int>> curr{{1, 0}};
        for (int u : adj_list[v]) {
            self(self, u);
            size[v] += size[u];
            curr.emplace_back(k[u], size[u]);
        }

        for (auto [t, s] : curr) k[v] += t;
        time += k[v];

        if (curr.size() == 1) return;

        sort(curr.begin() + 1, curr.end(), [](auto p1, auto p2) { return p1.first * p2.second < p2.first * p1.second; });
        vector<long long> suff(curr.size(), 0);
        for (int i = curr.size() - 1; i; i--) {
            suff[i - 1] = suff[i] + curr[i].second;
            time += suff[i - 1] * curr[i - 1].first;
        }
    };

    dfs(dfs);
    cout << time;
}
