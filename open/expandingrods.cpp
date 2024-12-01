#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double L, n, C;
    while (cin >> L >> n >> C && L != -1 && n != -1 && C != -1) {
        C *= n;

        double l = 0, r = M_PI / 2, m;
        while (l + 1e-10 < r && l + l * 1e-10 < r) {
            m = l + (r - l) / 2;

            if (m < (C + 1) * sin(m)) l = m;
            else r = m;
        }

        cout << fixed << setprecision(9) << (L * (C + 1)) * (1 - cos(m)) / (2 * m) << "\n";
    }
}
