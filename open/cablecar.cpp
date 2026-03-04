#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int d = sqrt(n) + 1;
    if (n - d < k) {
        cout << "NO";
        exit(0);
    }

    cout << "YES\n";
    for (int i = 1, j = 0; j < k; i++)
        if (i % d) {
            cout << i << " " << i + 1 << "\n";
            j++;
        }
    for (int i = 1; i <= k; i++) cout << i << " " << i + d << "\n";
}
