#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string c, s;
    cin >> n >> m >> c >> s;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<int> subtree_size(n, 1), prev(n, -1);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
                subtree_size[v] += subtree_size[u];
            }
    };
    dfs(dfs);

    auto sum = (long long) count_if(c.begin(), c.end(), [&](char ch) { return ch < s[0]; }) * n;
    vector<pair<int, int>> same;
    for (int i = 0; i < n; i++)
        if (c[i] == s[0]) same.emplace_back(i, -1);

    s.erase(s.begin());
    for (char ch : s) {
        sum += same.size();

        for (auto [v, p] : same)
            for (int u : adj_list[v])
                if (u != p)
                    if (c[u] < ch) sum += v == prev[u] ? subtree_size[u] : n - subtree_size[v];

        vector<pair<int, int>> temp;
        for (auto [v, p] : same)
            for (int u : adj_list[v])
                if (u != p)
                    if (c[u] == ch) temp.emplace_back(u, v);

        if (temp.empty()) break;
        same = temp;
    }
    cout << (long long) n * n - sum;
}
