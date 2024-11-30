#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && ~n && ~m) {
        if (n == m) {
            cout << "YES\n";
            for (int i = 1; i < n; i++) cout << i << " " << i + 1 << "\n";
            cout << 1 << " " << n << "\n";
            continue;
        }

        if (n * (n - 1) / 2 == m) {
            cout << "YES\n";
            for (int i = 1; i <= n; i++)
                for (int j = i + 1; j <= n; j++) cout << i << " " << j << "\n";
            continue;
        }

        if (!(n & 1) && m == n * n / 4) {
            cout << "YES\n";
            for (int i = 1; i <= n / 2; i++)
                for (int j = n / 2 + 1; j <= n; j++) cout << i << " " << j << "\n";
            continue;
        }

        cout << "NO\n";
    }
}
