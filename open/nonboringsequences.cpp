#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<int> prev(2e5 + 1), next(2e5 + 1);
    while (t--) {
        int n;
        cin >> n;

        unordered_map<int, int> indices;
        for (int i = 1; i <= n; i++) {
            int e;
            cin >> e;

            next[indices[e]] = i;
            prev[i] = indices[e];
            indices[e] = i;
        }
        for (auto [e, i] : indices) next[i] = n + 1;

        auto boring = [&](auto &&self, int l, int r) {
            if (l >= r) return false;
            for (int i = l, j = r; i <= j; i++, j--) {
                if (prev[i] < l && next[i] > r) return self(self, l, i - 1) || self(self, i + 1, r);
                if (prev[j] < l && next[j] > r) return self(self, l, j - 1) || self(self, j + 1, r);
            }
            return true;
        };
        cout << (boring(boring, 1, n) ? "boring\n" : "non-boring\n");
    }
}
