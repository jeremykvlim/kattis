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

        vector<int> indices(n + 1), pref(n + 1, 0);
        iota(indices.begin(), indices.end(), 0);
        auto dfs = [&](auto &&self, const vector<int> &indices, int b = 29) -> void {
            if (indices.size() <= 1) return;

            int u = indices[0];
            vector<int> l{u}, r;
            for (int j = 1; j < indices.size(); j++) {
                int v = indices[j];
                if (query(u, v) < b) l.emplace_back(v);
                else {
                    r.emplace_back(v);
                    pref[v] |= 1 << b;
                }
            }

            if (b) {
                self(self, l, b - 1);
                self(self, r, b - 1);
            }
        };
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
