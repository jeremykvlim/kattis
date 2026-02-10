#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b, c;
        cin >> a >> b >> c;

        adj_list[a].emplace_back(b, c);
        adj_list[b].emplace_back(a, c);
    }

    vector<int> in(n + 1, 0), out(n + 1, 0), order(n, 0), prev(n + 1, 0);
    int count = 0;
    auto dfs = [&](auto self, int v = 1) -> void {
        order[in[v] = count++] = v;
        for (auto [u, c] : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
            }
        out[v] = count;
    };
    dfs(dfs);

    int base = 0;
    vector<int> delta(n + 1, 0);
    for (int v = 1; v <= n; v++) {
        sort(adj_list[v].begin(), adj_list[v].end(), [&](auto p1, auto p2) { return p1.second < p2.second; });

        for (int l = 0, r = 1; l < adj_list[v].size(); l = r++) {
            for (; r < adj_list[v].size() && adj_list[v][l].second == adj_list[v][r].second; r++);

            if (l + 1 < r) {
                for (int i = l; i < r; i++) {
                    int u = adj_list[v][i].first;
                    if (u == prev[v]) {
                        base++;
                        delta[in[v]]--;
                        delta[out[v]]++;
                    } else {
                        delta[in[u]]++;
                        delta[out[u]]--;
                    }
                }
            }
        }
    }

    vector<int> sum(n + 1, 0);
    for (int i = 0; i < n; i++) sum[order[i]] = base += delta[i];

    vector<int> good;
    for (int v = 1; v <= n; v++)
        if (!sum[v]) good.emplace_back(v);

    cout << good.size() << "\n";
    for (int v : good) cout << v << "\n";
}
