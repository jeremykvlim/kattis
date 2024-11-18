#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> c(n + 1), count(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> c[i];

        count[c[i]]++;
    }

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b, i);
        adj_list[b].emplace_back(a, i);
    }

    vector<long long> same(n - 1);
    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> unordered_map<int, long long> {
        unordered_map<int, long long> freq_v;
        freq_v[c[v]] = 1;

        for (auto [u, i] : adj_list[v])
            if (u != prev) {
                auto freq_u = self(self, u, v);
                for (auto [color, freq] : freq_u) same[i] += freq * (count[color] - freq);

                if (freq_v.size() < freq_u.size()) swap(freq_v, freq_u);
                for (auto [color, freq] : freq_u) freq_v[color] += freq;
            }

        return freq_v;
    };
    dfs(dfs);
    for (int i = 0; i < n - 1; i++) cout << same[i] << '\n';
}
