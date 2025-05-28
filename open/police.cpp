#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int k = 0;
    vector<vector<int>> initial(n + 1, vector<int>(m + 1)), final(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            cin >> initial[i][j];

            k = max(k, initial[i][j]);
        }

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> final[i][j];

    if (k == n * m) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                if (initial[i][j] != final[i][j]) {
                    cout << -1;
                    exit(0);
                }
        cout << 0;
        exit(0);
    }

    DisjointSets dsu(n + 1);
    vector<vector<int>> row(2, vector<int>(k + 1, -1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            if (initial[i][j]) row[0][initial[i][j]] = i;
            if (final[i][j]) row[1][final[i][j]] = i;
        }

    vector<int> indices(k + 1, -1);
    vector<bool> full_row(n + 1, true), same_row(n + 1, true);
    for (int i = 1; i <= n; i++) {
        vector<int> undo;
        for (int j = 1, count = 1; j <= m; j++) {
            if (!initial[i][j]) full_row[i] = false;
            if (initial[i][j] != final[i][j]) same_row[i] = false;
            if (final[i][j]) {
                undo.emplace_back(final[i][j]);
                indices[final[i][j]] = count++;
            }
        }
        if (undo.empty()) continue;

        vector<int> seq;
        for (int j = 1; j <= m; j++)
            if (initial[i][j] && ~indices[initial[i][j]]) seq.emplace_back(indices[initial[i][j]]);

        int len = 0;
        vector<int> lis(seq.size() + 1, INT_MAX);
        lis[0] = 0;
        for (int j : seq) {
            int pos = lower_bound(lis.begin(), lis.end(), j) - lis.begin();
            lis[pos] = j;
            len = max(len, pos);
        }
        k -= len;
        for (int K : undo) indices[K] = -1;
    }

    for (int i = 1; i <= k; i++)
        if (row[0][i] != row[1][i]) {
            int r0_set = dsu.find(row[0][i]), r1_set = dsu.find(row[1][i]);
            if (dsu.unite(row[0][i], row[1][i]))
                if (!full_row[r1_set]) full_row[r0_set]= false;
        }

    vector<bool> visited(n + 1, false);
    for (int i = 1; i <= n; i++) {
        int i_set = dsu.find(i);
        if (!visited[i_set]) {
            if (!same_row[i]) k += full_row[i_set];
            visited[i_set] = true;
        }
    }
    cout << k;
}
