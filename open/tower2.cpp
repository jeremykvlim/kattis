#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, _;
    cin >> t >> _;

    mt19937_64 rng(random_device{}());
    while (t--) {
        int n;
        cin >> n;

        vector<array<int, 3>> qs;
        auto ask = [&]() {
            for (auto [x, y, z] : qs) cout << "? " << x << " " << y << " " << z << "\n";
            cout << flush;

            vector<int> masks;
            for (auto [x, y, z] : qs) {
                int r;
                cin >> r;

                int mask = 0;
                while (r--) {
                    int a, b;
                    cin >> a >> b;

                    mask |= a + b == x + y ? 1 : a + b == y + z ? 2 : 4;
                }
                masks.emplace_back(mask);
            }
            return masks;
        };

        vector<int> order(n), temp(n), left, right, zs;
        iota(temp.begin(), temp.end(), 0);
        auto partition = [&](int x, int y, int n) {
            zs.clear();
            for (int i = 0; i < n; i++)
                if (temp[i] != x && temp[i] != y) zs.emplace_back(temp[i]);

            left = {x};
            right = {y};
            for (int i = 0; i < zs.size(); i++) {
                qs = {{x, y, zs[i]}};
                auto masks = ask();
                if (masks[0] == 1) return false;
                (masks[0] >= 4 ? left : right).emplace_back(zs[i]);
            }
            return true;
        };

        auto sort_suff = [&](auto &&self, int l, int r) -> void {
            if (l + 1 == r) return;
            if (l + 2 == r) {
                order[l + 1] = temp[l + 1];
                return;
            }
            if (l + 3 == r) {
                int a = temp[l + 1], b = temp[l + 2];
                qs = {{a, b, order[l]}};
                auto masks = ask();
                if (masks[0] == 3) swap(a, b);
                order[l + 1] = a;
                order[l + 2] = b;
                return;
            }

            int m = l + (r - l) / 2;
            if (l == (n - 1) / 2 && r == n) m--;

            int x = order[l], y = order[(2 * l - m + n) % n];
            qs.clear();
            for (int i = l + 1; i < r; i++) qs.push_back({x, y, temp[i]});
            auto masks = ask();
            bool found = false;
            for (int i = 0, j = l + 1, k = m + 1; i < qs.size(); i++) {
                int z = qs[i][2];
                if (j == m && k == r) {
                    order[m] = z;
                    break;
                }

                if (found && (j == m || k == r)) (j == m ? temp[k++] : temp[j++]) = z;
                else if (popcount((unsigned) masks[i]) == 2) {
                    order[m] = z;
                    found = true;
                } else if (masks[i] != 1) temp[j++] = z;
                else temp[k++] = z;
            }
            self(self, l, m);
            self(self, m, r);
        };

        auto sort_pref = [&](auto &&self, int n) -> void {
            if (n == 2) {
                order[0] = temp[0];
                order[1] = temp[1];
                return;
            }

            for (;;) {
                int x = temp[rng() % n], y = temp[rng() % n];
                if (x != y && partition(x, y, n)) {
                    if (left.size() < right.size()) swap(left, right);
                    int k = left.size();
                    temp.clear();
                    temp.insert(temp.end(), left.begin(), left.end());
                    temp.insert(temp.end(), right.rbegin(), right.rend());
                    self(self, k);

                    qs = {{order[0], order[k - 1], temp[k]}};
                    if (k + 1 < n) qs.push_back({order[0], order[k - 1], temp[k + 1]});
                    auto masks = ask();
                    int mask = masks[0];
                    if (mask == 1) mask = masks[1];
                    if (mask == 4 || mask == 5 || (masks[0] == 1 && mask == 1)) reverse(order.begin(), order.begin() + k);
                    sort_suff(sort_suff, k - 1, n);
                    break;
                }
            }
        };

        for (;;) {
            int x = rng() % n, y = rng() % n;
            if (x != y && partition(x, y, n)) break;
        }

        int k = left.size();
        temp.clear();
        temp.insert(temp.end(), left.begin(), left.end());
        temp.insert(temp.end(), right.begin(), right.end());
        sort_pref(sort_pref, k);
        sort_suff(sort_suff, k - 1, n);

        cout << "! ";
        for (int x : order) cout << x << " ";
        cout << "\n" << flush;
    }
}
