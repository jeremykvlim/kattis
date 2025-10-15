#include <bits/stdc++.h>
using namespace std;

pair<int, unsigned __int128> bron_kerbosch(int n, vector<unsigned __int128> adj_list) {
    int max_clique_size = 0;
    unsigned __int128 clique_mask = 0;

    auto dfs = [&](auto &&self, unsigned __int128 p, unsigned __int128 r = 0) {
        if (!p) {
            int size = popcount(r);
            if (max_clique_size < size) {
                max_clique_size = size;
                clique_mask = r;
            }
            return;
        }

        if (popcount(r) + popcount(p) <= max_clique_size) return;

        auto lsb = [&](unsigned __int128 v) {
            return v ? ((v & ULLONG_MAX) ? countr_zero(v) : 64 + countr_zero(v >> 64)) : 0;
        };

        auto color_order = [&](unsigned __int128 p) -> vector<pair<int, int>> {
            int color = 0;
            vector<pair<int, int>> order;
            while (p) {
                color++;
                for (auto q = p; q;) {
                    int v = lsb(q);
                    auto mask = (unsigned __int128) 1 << v;
                    q &= q - 1;
                    order.emplace_back(v, color);
                    p &= ~mask;
                    q &= ~adj_list[v];
                }
            }
            reverse(order.begin(), order.end());
            return order;
        };

        for (auto [v, color] : color_order(p)) {
            if (popcount(r) + color <= max_clique_size) return;
            auto mask = (unsigned __int128) 1 << v;
            self(self, p & adj_list[v], r | mask);
            p &= ~mask;
        }
    };
    dfs(dfs, ((unsigned __int128) 1 << n) - 1);

    return {max_clique_size, clique_mask};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, e;
    cin >> V >> e;

    vector<unsigned __int128> adj_list(V, 0);
    while (e--) {
        int u, v;
        cin >> u >> v;

        adj_list[u] |= (unsigned __int128) 1 << v;
        adj_list[v] |= (unsigned __int128) 1 << u;
    }
    cout << bron_kerbosch(V, adj_list).first;
}
