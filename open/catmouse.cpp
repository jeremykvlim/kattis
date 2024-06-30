#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int R, t, j;
        cin >> R >> t >> j;

        if (t >= j) {
            cout << "YES\n";
            continue;
        }

        double l = 0, r = M_PI_2, m;
        while (l + 1e-8 < r && l + l * 1e-8 < r) {
            m = l + (r - l) / 2;

            if (tan(m) > M_PI + m) r = m;
            else l = m;
        }

        cout << (t * (M_PI + m) / sin(m) >= j ? "YES\n" : "NO\n");
    }
}
