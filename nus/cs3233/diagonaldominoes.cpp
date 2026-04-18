#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    
    while (t--) {
        int m, n;
        cin >> m >> n;

        cout << (n - m) * (n + m - 1) / 2 << "\n";
        for (int a = m + 1; a <= n; a++)
            for (int b = 1; b <= 2 * a - 2; b += 2) cout << a << " " << b << " " << a << " " << b + 1 << "\n";
    }
}
