#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int c = 0, offset = 0;
    for (int i = 1; i <= n; i++) {
        int curr = min(n - i, n - (n + i - 1) / i);
        if (curr > c) {
            c = curr;
            offset = i;
        }
    }

    if (c < k) {
        cout << "NO";
        exit(0);
    }

    cout << "YES\n";
    for (int i = 1, j = 0; j < k; i++)
        if (i % offset) {
            cout << i << " " << i + 1 << "\n";
            j++;
        }
    for (int i = 1; i <= k; i++) cout << i << " " << i + offset << "\n";
}
