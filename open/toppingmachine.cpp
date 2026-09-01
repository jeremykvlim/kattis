#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    if (n * n < (1 << k)) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    cout << "POSSIBLE\n";
    vector<int> rows(n, 0), cols(n, 0), diags(2 * n - 1, 0), antidiags(2 * n - 1, 0);
    if (k == 1) rows[0] = 1;
    else if (!(k & 1)) {
        for (int i = 0, s = k / 2; i < 1 << s; i++) {
            rows[i] = i;
            cols[i] = i << s;
        }
    } else {
        for (int i = 0, s = (k - 3) / 2; i < 3 << s; i++) {
            rows[i] = (i % 3 ? 0 : 5) | (i / 3 << 3);
            cols[i] = ((1 << (i % 3)) & 3) | (i / 3 << (s + 3));
        }

        for (int i = 0; i < 2 * n - 1; i++) {
            if (i % 3 == n % 3) diags[i] = 2;
            if (!(i % 3)) antidiags[i] = 4;
        }
    }

    auto print = [&](unsigned mask) {
        cout << popcount(mask) << " ";
        for (int c = 0; c < k; c++)
            if ((mask >> c) & 1) cout << c + 1 << " ";
        cout << "\n";
    };
    for (auto &masks : {rows, cols, diags, antidiags})
        for (int mask : masks) print(mask);
}
