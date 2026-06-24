#include <bits/stdc++.h>
using namespace std;

pair<int, unsigned> bron_kerbosch(int n, vector<unsigned> adj_list) {
    int max_clique_size = 0;
    unsigned clique_mask = 0;

    auto dfs = [&](auto &&self, unsigned p, unsigned r = 0) {
        if (!p) {
            int size = popcount(r);
            if (max_clique_size < size) {
                max_clique_size = size;
                clique_mask = r;
            }
            return;
        }

        if (popcount(r) + popcount(p) <= max_clique_size) return;

        auto color_order = [&](unsigned p) -> vector<pair<int, int>> {
            int color = 0;
            vector<pair<int, int>> order;
            while (p) {
                color++;
                for (auto q = p; q;) {
                    int v = countr_zero(q);
                    auto mask = 1U << v;
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
            auto mask = 1U << v;
            self(self, p & adj_list[v], r | mask);
            p &= ~mask;
        }
    };
    dfs(dfs, (1U << n) - 1);

    return {max_clique_size, clique_mask};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<unsigned> friends(n, 0);
    while (m--) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        if (a && b) {
            friends[a] |= 1U << b;
            friends[b] |= 1U << a;
        }
    }

    vector<int> degree(n, 0);
    vector<unsigned> adj_list(n, 0);
    for (int i = 1; i < n; i++) degree[i] = popcount(adj_list[i] = ((1U << n) - 1) & ~(friends[i] | 1U | (1U << i)));

    for (int colors = bron_kerbosch(n, adj_list).first;; colors++) {
        vector<int> color(n, -1);
        vector<unsigned> color_mask(colors, 0);
        auto dfs = [&](auto &&self, int used = 0) -> bool {
            int v = -1, pc = -1, d = -1;
            for (int u = 1; u < n; u++)
                if (color[u] == -1) {
                    auto mask = 0U;
                    for (int c = 0; c < used; c++)
                        if (adj_list[u] & color_mask[c]) mask |= 1U << c;

                    int p = popcount(mask);
                    if (pc < p || (pc == p && d < degree[u])) {
                        pc = p;
                        d = degree[u];
                        v = u;
                    }
                }
            if (v == -1) return true;

            for (int c = 0; c < used; c++)
                if (!(adj_list[v] & color_mask[c])) {
                    color[v] = c;
                    color_mask[c] |= 1U << v;
                    if (self(self, used)) return true;
                    color_mask[c] ^= 1U << v;
                    color[v] = -1;
                }

            if (colors > used) {
                color[v] = used;
                color_mask[used] |= 1U << v;
                if (self(self, used + 1)) return true;
                color_mask[used] ^= 1U << v;
                color[v] = -1;
            }
            return false;
        };
        if (dfs(dfs)) {
            int h = *max_element(color.begin() + 1, color.end()) + 1;
            cout << h << "\n";
            for (int c = 0; c < h; c++) {
                vector<int> group{1};
                for (int v = 1; v < n; v++)
                    if (color[v] == c) group.emplace_back(v + 1);

                cout << group.size() << " ";
                for (int v : group) cout << v << " ";
                cout << "\n";
            }
            exit(0);
        }
    }
}
