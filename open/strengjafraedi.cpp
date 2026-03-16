#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> adj_masks(n, 0);
    for (int i = 0; i < n; i++) adj_masks[i] |= 1 << i;

    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_masks[a - 1] |= 1 << (b - 1);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if ((adj_masks[j] >> i) & 1)
                for (int k = 0; k < n; k++)
                    if ((adj_masks[i] >> k) & 1) adj_masks[j] |= 1 << k;

    vector<int> memo(1 << n, -1);
    auto dfs = [&](auto &&self, int mask) {
        if (!mask) return 0;
        if (memo[mask] != -1) return memo[mask];

        for (int i = 0; i < n; i++)
            if (((mask >> i) & 1))
                if (!self(self, mask & ~adj_masks[i])) return memo[mask] = 1;
        return memo[mask] = 0;
    };
    cout << (dfs(dfs, (1 << n) - 1) ? "Atli" : "Bergur");
}
