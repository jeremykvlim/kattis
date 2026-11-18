#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> f(n - 1), t(n - 1);
    for (int &fi : f) cin >> fi;
    for (int &ti : t) cin >> ti;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        adj_list[f[i]].emplace_back(t[i]);
        adj_list[t[i]].emplace_back(f[i]);
    }

    vector<int> len(n, 1), heavy(n, -1);
    auto hld = [&](auto &&self, int v = 0, int prev = -1) -> int {
        int longest = 0;
        for (int u : adj_list[v])
            if (u != prev) {
                int l = self(self, u, v);
                if (longest < l) {
                    longest = l;
                    heavy[v] = u;
                }
            }
        return len[v] = longest + 1;
    };
    hld(hld);

    auto cities = 0LL;
    vector<int> freq(len[0], 0);
    auto dfs = [&](auto &&self, int v = 0, int prev = -1, int i = 0) -> void {
        freq[i] = 1;
        if (~heavy[v]) self(self, heavy[v], v, i + 1);
        for (int u : adj_list[v])
            if (u != prev && u != heavy[v]) {
                int j = freq.size();
                freq.resize(j + len[u]);
                self(self, u, v, j);
                for (int l = max(0, k - len[v]); l < min(len[u], k - 1); l++) cities += (long long) freq[j + l] * freq[i + k - l - 1];
                for (int l = 0; l < len[u]; l++) freq[i + l + 1] += freq[j + l];
            }

        if (k < len[v]) cities += freq[i + k];
    };
    dfs(dfs);
    cout << cities;
}
