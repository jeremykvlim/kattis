#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V;
    cin >> V;

    vector<array<int, 3>> edges(V - 1);
    vector<vector<pair<int, int>>> adj_list(V + 1);
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;

        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    vector<int> in(V + 1, 0), out(V + 1, 0), prev(V + 1, 0);
    vector<long long> subtree_sum(V + 1, 0);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 1) -> void {
        in[v] = count++;
        for (auto [u, w] : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                self(self, u);
                subtree_sum[v] += subtree_sum[u] + w;
            }
        out[v] = count;
    };
    dfs(dfs);

    int T;
    cin >> T;

    auto t = subtree_sum[1] - subtree_sum[V];
    while (T--) {
        int s;
        cin >> s;
        s--;

        auto [u, v, w] = edges[s];
        int c = prev[u] == v ? u : v;
        if (in[c] <= in[V] && in[V] < out[c]) cout << "Kemst ekki\n";
        else if (in[V] <= in[c] && in[c] < out[V]) cout << t << "\n";
        else cout << t - subtree_sum[c] - edges[s][2] << "\n";
    }
}
