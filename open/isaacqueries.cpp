#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<vector<int>> memo(n + 1, vector<int>(n + 1, -2));
        auto query = [&](int u, int v) -> int {
            if (memo[u][v] != -2) return memo[u][v];
            cout << "? " << u + 1 << " " << v << "\n" << flush;
            int lg;
            cin >> lg;
            return memo[u][v] = lg;
        };

        vector<int> pref(n + 1, 0);
        auto dfs = [&](auto &&self, const vector<int> &indices, int b = 29) -> void {
            if (indices.size() <= 1) return;

            int u = indices.front(), v = indices.back();
            vector<int> l, r;
            l.emplace_back(u);
            int side = 1;
            if (query(u, v) < b) {
                l.emplace_back(v);
                side = 0;
            } else r.emplace_back(v);

            for (int i : indices)
                if (i != u && i != v) {
                    if (u + v < 2 * i) (query(u, i) < b ? l : r).emplace_back(i);
                    else (query(i, v) < b ? (!side ? l : r) : (!side ? r : l)).emplace_back(i);
                }
            for (int i : r) pref[i] += 1 << b;

            if (b) {
                sort(l.begin(), l.end());
                sort(r.begin(), r.end());
                self(self, l, b - 1);
                self(self, r, b - 1);
            }
        };
        vector<int> indices(n + 1);
        iota(indices.begin(), indices.end(), 0);
        dfs(dfs, indices);

        cout << "!\n";
        for (int u = 1; u <= n; u++) {
            for (int v = u; v <= n; v++) {
                int x = pref[u - 1] ^ pref[v];
                cout << (x ? __lg(x) : -1) << " ";
            }
            cout << "\n";
        }
        cout << flush;
    }
}