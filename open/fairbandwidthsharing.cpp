#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    vector<double> a(n), b(n), d(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i] >> d[i];

    double l = 0, r = 1e6, m;
    while (l + 1e-12 < r && l + l * 1e-12 < r) {
        m = l + (r - l) / 2;

        double total = 0;
        for (int i = 0; i < n; i++) total += max(a[i], min(b[i], d[i] * m));

        if (total > t) r = m;
        else l = m;
    }

    for (int i = 0; i < n; i++) cout << fixed << setprecision(7) << max(a[i], min(b[i], d[i] * l)) << "\n";
}
