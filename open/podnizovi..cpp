#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, b, m;
    cin >> n >> k >> b >> m;

    vector<int> a(n + 1);
    int biggest = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        biggest = max(biggest, a[i]);
    }

    vector<vector<int>> sparse(__lg(biggest) + 1, vector<int>(biggest + 1, 0)), indices(biggest + 1);
    for (int i = 1; i <= n; i++) {
        sparse[0][a[i]] = i;
        indices[a[i]].emplace_back(i);
    }

    for (int i = 1; i < sparse.size(); i++)
        for (int j = 1; j + (1 << i) - 1 <= biggest; j++)
            sparse[i][j] = max(sparse[i - 1][j], sparse[i - 1][j + (1 << (i - 1))]);

    auto dfs = [&](auto &&self, vector<int> curr = {-1}, int v = 0) {
        for (int j = 1; j <= biggest; j++) {
            for (int i = sparse.size() - 1; ~i; i--)
                if (sparse[i][j] && sparse[i][j] <= curr[0]) {
                    j += (1 << i);
                    if (j > biggest) return;
                }

            int h = ((long long) v * b % m + j) % m;
            vector<int> next;
            for (int i = upper_bound(indices[j].begin(), indices[j].end(), curr[0]) - indices[j].begin(); i < indices[j].size(); i++)
                for (int l : curr) {
                    if (l >= indices[j][i]) break;

                    cout << h << "\n";
                    if (!--k) exit(0);

                    next.emplace_back(indices[j][i]);
                }

            if (!next.empty()) self(self, next, h);
        }
    };

    dfs(dfs);
}
