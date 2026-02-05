#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int m;
        cin >> m;

        vector<unsigned long long> adj_masks(36);
        while (m--) {
            int s, d;
            cin >> s >> d;
            s--;
            d--;

            adj_masks[s] |= 1ULL << d;
        }

        int k = 2;
        auto dfs = [&](auto &&self, unsigned long long mask, int chosen = 0, int i = 0) -> bool {
            if (popcount(mask) < k) return false;
            if (chosen == k) return true;
            if (i == adj_masks.size()) return false;
            if (self(self, mask, chosen, i + 1)) return true;
            return self(self, mask & adj_masks[i], chosen + 1, i + 1);
        };
        for (; dfs(dfs, (1ULL << 36) - 1); k++);
        cout << k - 1 << "\n";
    }
}
