#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        double m, b, d, t;
        cin >> m >> b >> d >> t;

        auto a = atan(b / m), l = 11 * b / (m * 2) - 1 / (2 * cos(a)) + t * b - 2, r = l + 1 / cos(a) + 2;
        for (int k = 0; k < 10; k++) {
            auto pos = d + 4 * k;
            if (l <= pos && pos <= r) {
                cout << "Collision with bicycle " << k + 1 << "\n";
                goto next;
            } else if (!k && r < pos) {
                cout << "Max beats the first bicycle\n";
                goto next;
            } else if (r < pos) {
                cout << "Max crosses safely after bicycle " << k << "\n";
                goto next;
            }
        }
        cout << "Max crosses safely after bicycle 10\n";
        next:;
    }
}
