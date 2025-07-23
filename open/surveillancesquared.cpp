#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<long long> x(n), y(n), a(n), b(n);
        for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> a[i] >> b[i];

        for (int i = 0; i < n; i++) {
            int count = 0;
            for (int j = 0; j < n; j++)
                if (i != j) {
                    auto dx = x[j] - x[i], dy = y[j] - y[i], dot = a[i] * dx + b[i] * dy;
                    if (dot > 0) count += 2 * dot * dot >= (dx * dx + dy * dy) * (a[i] * a[i] + b[i] * b[i]);
                }
            cout << count << "\n";
        }
        cout << "\n";
    }
}
