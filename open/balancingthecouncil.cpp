#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    if (!(m & 1)) {
        cout << "Yes";
        exit(0);
    }

    int k = n >> countr_zero((unsigned) n);
    if (k == 1) {
        cout << "No";
        exit(0);
    }

    int p = -1;
    for (int d = 3; d * d <= k; d += 2)
        if (!(k % d)) {
            p = d;
            break;
        }
    if (!~p) p = k;
    cout << (p <= m && m <= n - p ? "Yes" : "No");
}
