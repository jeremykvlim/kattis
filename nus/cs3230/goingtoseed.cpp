#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int t = -1, l1 = 1, r = n + 1;
    while (!~t) {
        int r1 = l1 + (r - l1) / 2, l2 = l1 + (r1 - l1) / 2, r2 = r1 + (r - r1) / 2;
        cout << "Q " << l1 << " " << r1 - 1 << " " << l2 << " " << r2 - 1 << "\n" << flush;

        bool w1, w2;
        cin >> w1 >> w2;

        auto update = [&](int L1, int L2, int R1, int R2) {
            t = w1 ? (L1 + 1 == L2 ? L1 : -1) : (R1 + 1 == R2 ? R1 : -1);
            l1 = (w1 ? L1 : R1) - 1;
            r = (w1 ? L2 : R2) + 1;

            l1 = max(l1, 1);
            r = min(r, n + 1);
        };

        if (w1 == w2) update(l2, r1, r2, r);
        else update(l1, l2, r1, r2);
    }

    cout << "A " << t;
}
