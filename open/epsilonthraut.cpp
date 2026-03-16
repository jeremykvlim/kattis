#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double a, b;
    cin >> a >> b;

    if (a + 1 > b) {
        cout << "Engin lausn!";
        exit(0);
    }

    int n = 1;
    for (; n <= 1e5; n++)
        if (n * log1p(a / n) >= log(b)) break;

    if (n == 1e5 + 1) {
        cout << "Engin lausn!";
        exit(0);
    }

    cout << n << "\n";
    double l = 0, r = a / n, m;
    while (l + 1e-7 < r && l + l * 1e-7 < r) {
        m = (l + r) / 2;

        if ((n - 1) * log1p(m) + log1p(a - (n - 1) * m) < log(b)) l = m;
        else r = m;
    }

    for (int _ = 0; _ < n - 1; _++) cout << fixed << setprecision(11) << r << " ";
    cout << fixed << setprecision(11) << a - (n - 1) * r;
}
