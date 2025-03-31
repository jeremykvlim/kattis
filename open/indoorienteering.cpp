#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long L;
    cin >> n >> L;

    vector<vector<long long>> d(n, vector<long long>(n));
    for (auto &row : d)
        for (auto &dij : row) cin >> dij;

    for (int k = 0; k < n - 1; k++) {
        vector<vector<long long>> len(1 << (n - 1));
        unsigned m = ((1 << (n - 1)) - 1) ^ (1 << k);
        auto dfs = [&](auto &&self, int i, unsigned visited = 0, long long w = 0) {
            int v = popcount(visited);
            if (v >= n / 2 - 1) {
                len[visited].emplace_back(d[k][i] + w);
                if (v == n - n / 2 - 1) return;
            }

            for (auto unvisited = visited ^ m; unvisited;) {
                int j = countr_zero(unvisited);
                unvisited ^= 1 << j;
                self(self, j, visited | (1 << j), d[i][j] + w);
            }
        };
        dfs(dfs, n - 1);

        for (int mask = 0; mask < 1 << (n - 1); mask++)
            if (mask <= (mask ^ m) && len[mask].size()) {
                sort(len[mask ^ m].begin(), len[mask ^ m].end());
                for (auto l : len[mask]) {
                    auto it = lower_bound(len[mask ^ m].begin(), len[mask ^ m].end(), L - l);
                    if (it != len[mask ^ m].end() && *it == L - l) {
                        cout << "possible";
                        exit(0);
                    }
                }
            }
    }
    cout << "impossible";
}
