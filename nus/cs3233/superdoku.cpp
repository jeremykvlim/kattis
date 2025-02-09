#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> grid(n + 1, vector<int>(n + 1));
    vector<vector<bool>> row(n + 1, vector<bool>(n + 1)), col(n + 1, vector<bool>(n + 1));

    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= n; j++) {
            cin >> grid[i][j];

            if (row[grid[i][j]][i] || col[grid[i][j]][j]) {
                cout << "no";
                exit(0);
            }

            row[grid[i][j]][i] = col[grid[i][j]][j] = true;
        }

    for (int i = k + 1; i <= n; i++) {
        vector<int> matched(n + 1);
        for (int j = 1; j <= n; j++) {
            __int128 visited = 0;
            auto dfs = [&](auto &&self, int i, int j) -> bool {
                for (int v = 1; v <= n; v++)
                    if (!((visited >> v) & 1) && !col[j][v]) {
                        visited |= (__int128) 1 << v;
                        if (!matched[v] || self(self, i, matched[v])) {
                            col[matched[v]][v] = false;
                            col[j][v] = true;
                            grid[i][v] = matched[v] = j;
                            return true;
                        }
                    }

                return false;
            };

            if (!dfs(dfs, i, j)) {
                cout << "no";
                exit(0);
            }
        }
    }

    cout << "yes\n";
    for (int i = 1; i <= n; i++) {
        cout << grid[i][1];
        for (int j = 2; j <= n; j++) cout << " " << grid[i][j];
        cout << "\n";
    }
}
