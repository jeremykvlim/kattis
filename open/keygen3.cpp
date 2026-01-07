#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> p(n + 1, 0);
    vector<vector<int>> permutations;
    vector<bool> visited(n + 1, false);
    auto dfs = [&](auto &&self, int l, int r) -> void {
        if (permutations.size() == 2000) return;

        int cycles = 0;
        fill(visited.begin(), visited.end(), false);
        for (int i = 1; i <= n; i++) {
            if (!p[i] || visited[i]) continue;
            visited[i] = true;
            int j = p[i];
            for (; j && i != j; j = p[j]) visited[j] = true;
            cycles += i == j;
        }

        int len = max(r - l + 1, 0);
        if (cycles + !!len > m || cycles + len < m) return;

        if (l > r) {
            permutations.emplace_back(p);
            return;
        }

        p[l] = n - r + l;
        self(self, l + 1, r);
        p[l] = 0;
        if (l < r) {
            p[r] = n - r + l;
            self(self, l, r - 1);
            p[r] = 0;
        }
    };
    dfs(dfs, 1, n);

    cout << permutations.size() << "\n";
    for (auto perm : permutations) {
        for (int i = 1; i <= n; i++) cout << perm[i] << " ";
        cout << "\n";
    }
}