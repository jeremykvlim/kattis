#include <bits/stdc++.h>
using namespace std;

int bron_kerbosch(int n, vector<unsigned __int128> adj_list) {
    int cliques = 0;

    auto dfs = [&](auto &&self, unsigned __int128 p, unsigned __int128 x = 0, unsigned __int128 r = 0) {
        if (!p && !x) {
            cliques++;
            return;
        }

        auto lsb = [&](unsigned __int128 v) {
            return v ? ((v & ULLONG_MAX) ? __builtin_ctzll(v) : 64 + __builtin_ctzll(v >> 64)) : 0;
        };
        int pivot = lsb(p | x);
        if (pivot >= n) return;

        auto candidates = p & ~adj_list[pivot];
        while (candidates && cliques <= 1e3) {
            int v = lsb(candidates);
            r |= (unsigned __int128) 1 << v;
            self(self, p & adj_list[v], x & adj_list[v], r);
            p &= ~((unsigned __int128) 1 << v);
            x |= (unsigned __int128) 1 << v;
            candidates &= ~((unsigned __int128) 1 << v);
        }
    };
    dfs(dfs, ((unsigned __int128) 1 << n) - 1);

    return cliques;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m) {
        vector<unsigned __int128> adj_list(n, 0);
        while (m--) {
            int a, b;
            cin >> a >> b;

            adj_list[a - 1] |= (unsigned __int128) 1 << (b - 1);
            adj_list[b - 1] |= (unsigned __int128) 1 << (a - 1);
        }

        int cliques = bron_kerbosch(n, adj_list);
        if (cliques <= 1e3) cout << cliques << "\n";
        else cout << "Too many maximal sets of friends.\n";
    }
}
