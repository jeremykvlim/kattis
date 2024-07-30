#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> a(2 * n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];

            a[i + n] = a[i];
        }

        int count = 0;
        vector<int> b(2 * n + 1, 0);
        for (int i = 2 * n - 1; ~i; i--) {
            b[i] = min(b[i], b[i + 1] + a[i]);
            if (i < n && b[i] < 0) count++;
        }

        cout << n - count << "\n";
    }
}
