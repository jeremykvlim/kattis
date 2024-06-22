#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n), count(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int aij;
            cin >> aij;

            if (aij) adj_list[i].emplace_back(j);
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) count[i][j] = adj_list[j].size();

    vector<vector<int>> next(n, vector<int>(n));
    vector<vector<bool>> visited_r(n, vector<bool>(n, false)), visited_c(n, vector<bool>(n, false));
    auto dfs = [&](auto &&self, int i, int j, bool r) -> void {
        (r ? visited_r : visited_c)[i][j] = true;

        if (r) {
            for (int k : adj_list[i])
                if (!visited_c[k][j]) {
                    next[k][j] = i;
                    self(self, k, j, false);
                }

            if (!visited_c[i][j]) {
                next[i][j] = i;
                self(self, i, j, false);
            }
        } else
            for (int k : adj_list[j])
                if (!--count[i][k] && !visited_r[i][k]) self(self, i, k, true);
    };

    for (int i = 0; i < n; i++) {
        if (!visited_c[i][i]) dfs(dfs, i, i, false);
        if (!visited_r[i][i]) dfs(dfs, i, i, true);
    }

    int i = 0;
    for (; i < n; i++) {
        for (int j = 0; j < n; j++)
            if (!visited_c[i][j]) goto next;

        cout << i << "\n" << flush;
        goto possible;
        next:;
    }
    cout << "-1\n" << flush;
    exit(0);
    possible:;

    for (;;) {
        int r;
        cin >> r;

        if (!~r) exit(0);

        i = next[i][r];
        cout << i << "\n" << flush;
    }
}
