#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<bool, 3>> types(n, {true, true, true});
    vector<vector<int>> adj_list(n);
    while (m--) {
        int i, j;
        char c;
        cin >> i >> j >> c;

        if (c != 'x') types[i - 1][0] = types[j - 1][0] = false;

        if (c == '-') types[i - 1][1] = types[j - 1][1] = false;
        else if (c == '+') types[i - 1][2] = types[j - 1][2] = false;
        else if (c == '=') {
            adj_list[i - 1].emplace_back(j - 1);
            adj_list[j - 1].emplace_back(i - 1);
        }
    }

    auto dfs = [&](auto &&self, int i, bool plus) -> void {
        types[i][plus ? 1 : 2] = false;
        for (int j : adj_list[i])
            if (types[j][plus ? 2 : 1]) self(self, j, !plus);
    };

    vector<int> colour(n, -1);
    auto bipartite = [&](auto &&self, int i) -> bool {
        for (int j : adj_list[i]) {
            if (colour[j] == -1) {
                colour[j] = colour[i] ^ 1;
                if (!self(self, j)) return false;
            }
            if (colour[i] == colour[j]) return false;
        }

        return true;
    };

    for (int i = 0; i < n; i++) {
        auto [x, plus, minus] = types[i];
        if (x) continue;

        if (!plus) dfs(dfs, i, true);
        if (!minus) dfs(dfs, i, false);

        if (colour[i] == -1) {
            colour[i] = 0;

            if (!bipartite(bipartite, i)) {
                dfs(dfs, i, true);
                dfs(dfs, i, false);
            }
        }
    }

    for (auto [x, plus, minus] : types) {
        if (x) cout << 'x';
        else if (plus && minus || !plus && !minus) cout << '=';
        else if (plus) cout << '+';
        else cout << '-';
    }
}
