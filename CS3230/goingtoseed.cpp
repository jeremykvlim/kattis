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

        if (w1 == w2) {
            t = w1 ? (l2 == r1 - 1 ? l2 : -1) : (r2 == r - 1 ? r2 : -1);
            l1 = w1 ? l2 - 1 : r2 - 1;
            r = w1 ? r1 + 1 : min(r, n) + 1;
        } else {
            t = w1 ? (l1 == l2 - 1 ? l1 : -1) : (r1 == r2 - 1 ? r1 : -1);
            l1 = w1 ? max(l1 - 1, 1) : r1 - 1;
            r = w1 ? l2 + 1 : r2 + 1;
        }
    }

    cout << "A " << t;
}
