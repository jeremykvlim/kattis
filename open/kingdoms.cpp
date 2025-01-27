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

        vector<vector<long long>> gold(n, vector<long long>(n));
        for (auto &row : gold)
            for (auto &num : row) cin >> num;

        vector<bool> visited(1 << n, false);
        vector<long long> sum(n, 0);
        for (auto &row : gold)
            for (int i = 0; i < n; i++) sum[i] += row[i];

        auto dfs = [&](auto &&self, int mask) -> void {
            visited[mask] = true;
            for (int i = 0; i < n; i++)
                if ((mask >> i) & 1 && sum[i] < 0 && !visited[mask - (1 << i)]) {
                    for (int j = 0; j < n; j++)
                        if ((mask >> j) & 1) sum[j] -= gold[i][j];

                    self(self, mask - (1 << i));
                    for (int j = 0; j < n; j++)
                        if ((mask >> j) & 1) sum[j] += gold[i][j];
                }
        };

        dfs(dfs, (1 << n) - 1);

        vector<int> survivors;
        for (int i = 0; i < n; i++)
            if (visited[1 << i]) survivors.emplace_back(i + 1);

        if (survivors.empty()) {
            cout << "0\n";
            continue;
        }

        for (int i : survivors) cout << i << " ";
        cout << "\n";
    }
}
