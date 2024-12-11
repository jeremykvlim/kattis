#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    auto sum = 0LL;
    int biggest = -1;
    vector<vector<int>> table(n, vector<int>(n));
    for (auto &row : table)
        for (int &e : row) {
            cin >> e;

            sum += e;
            biggest = max(biggest, e);
        }

    vector<vector<pair<int, int>>> indices(2 * biggest + 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i + 1 < n) indices[table[i][j] + table[i + 1][j]].emplace_back(i * n + j, (i + 1) * n + j);
            if (j + 1 < n) indices[table[i][j] + table[i][j + 1]].emplace_back(i * n + j, i * n + j + 1);
        }

    vector<int> overlaps;
    vector<pair<int, int>> largest;
    for (int i = 2 * biggest; ~i; i--)
        while (!indices[i].empty() && largest.size() <= 7 * (k - 1)) {
            overlaps.emplace_back(i);
            largest.emplace_back(indices[i].back());
            indices[i].pop_back();
        }

    int covered = 0, c = 0;
    vector<bool> visited(n * n, false);
    auto dfs = [&](auto &&self, int remaining, int l = 0) {
        covered = max(covered, c);
        if (!remaining || l >= largest.size() || covered >= c + overlaps[l] * remaining) return;

        for (int i = l; i < largest.size(); i++) {
            auto [u, v] = largest[i];

            if (!visited[u] && !visited[v]) {
                visited[u] = true;
                visited[v] = true;
                c += overlaps[i];
                self(self, remaining - 1, i + 1);
                visited[u] = false;
                visited[v] = false;
                c -= overlaps[i];
            }
        }
    };
    dfs(dfs, k);
    cout << sum - covered;
}
