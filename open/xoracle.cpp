#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    for (int i = 2; i <= n; i++) cout << "? " << 1 << " " << i << "\n";
    cout << flush;

    vector<int> xs(n + 1, 0);
    vector<bool> seen(n + 1, false);
    for (int i = 2; i <= n; i++) {
        cin >> xs[i];

        seen[xs[i]] = true;
    }

    int lg = __lg(n) + 1;
    vector<array<int, 2>> count(lg, {0, 0});
    for (int j = 0; j < lg; j++)
        for (int i = 1; i <= n; i++) count[j][(xs[i] >> j) & 1]++;

    for (int x = 1; x < n; x++)
        if (!seen[x]) {
            int sum = 0;
            for (int i = 0; i < lg; i++) sum += count[i][!((x >> i) & 1)] << i;
            if (sum == 2 * n - 2) {
                cout << "! ";
                for (int i = 1; i <= n; i++) cout << (x ^ xs[i]) << " ";
                exit(0);
            }
        }
}
