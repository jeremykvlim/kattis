#include <bits/stdc++.h>
using namespace std;

int bron_kerbosch(int n, vector<unsigned __int128> adj_list) {
    int max_clique_size = 0;

    auto dfs = [&](auto &&self, unsigned __int128 p, unsigned __int128 x = 0, unsigned __int128 r = 0) {
        if (!p && !x) {
            int size = popcount(r);
            max_clique_size = max(max_clique_size, size);
            return;
        }

        auto lsb = [&](unsigned __int128 v) {
            return v ? ((v & ULLONG_MAX) ? countr_zero(v) : 64 + countr_zero(v >> 64)) : 0;
        };
        int pivot = lsb(p | x);

        auto candidates = p & ~adj_list[pivot];
        while (candidates) {
            int v = lsb(candidates);
            auto mask = (unsigned __int128) 1 << v;
            self(self, p & adj_list[v], x & adj_list[v], r | mask);
            p &= ~mask;
            x |= mask;
            candidates &= ~mask;
        }
    };
    dfs(dfs, ((unsigned __int128) 1 << n) - 1);

    return max_clique_size;
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
    cout << bron_kerbosch(V, adj_list);
}
