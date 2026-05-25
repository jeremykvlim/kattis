#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, k;
    cin >> t >> k;

    mt19937 rng(random_device{}());
    while (t--) {
        int n;
        cin >> n;

        auto ask = [&](int x, int y, int z) -> int {
            cout << "? " << x << " " << y << " " << z << "\n" << flush;

            int r;
            cin >> r;

            int mask = 0;
            while (r--) {
                int a, b;
                cin >> a >> b;

                int sum = a + b;
                if (sum == x + y) mask |= 1;
                if (sum == y + z) mask |= 2;
                if (sum == z + x) mask |= 4;
            }
            return mask;
        };

        auto dfs = [&](auto &&self, auto indices) -> vector<int> {
            int m = indices.size();
            if (m <= 2) return indices;
            if (m == 3) {
                int mask = ask(indices[0], indices[1], indices[2]);
                if (mask == 5) return {indices[1], indices[0], indices[2]};
                if (mask == 6) return {indices[0], indices[2], indices[1]};
                return indices;
            }

            for (;;) {
                int i = rng() % m, j = rng() % m;
                for (; i == j; j = rng() % m);

                int x = indices[i], y = indices[j];
                vector<int> left{x}, right{y};
                bool valid = true;
                for (int z : indices)
                    if (x != z && y != z) {
                        int mask = ask(x, y, z);
                        if (mask & 1) {
                            valid = false;
                            break;
                        }

                        (((mask & 4) && !(mask & 2)) ? left : right).emplace_back(z);
                    }
                if (!valid) continue;

                auto l = self(self, left), r = self(self, right);
                auto ordered = [&]() -> bool {
                    if (l.size() > 1) return ask(l[l.size() - 2], l[l.size() - 1], r[0]) & 2;
                    return ask(l[0], r[0], r[1]) & 1;
                };
                if (!ordered()) {
                    reverse(r.begin(), r.end());
                    if (!ordered()) {
                        reverse(l.begin(), l.end());
                        if (!ordered()) reverse(r.begin(), r.end());
                    }
                }
                l.insert(l.end(), r.begin(), r.end());
                return l;
            }
        };

        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);

        auto order = dfs(dfs, indices);
        cout << "! ";
        for (int x : order) cout << x << " ";
        cout << "\n" << flush;
    }
}
