#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    while (cin >> k && k) {
        double vb, vw, vr, vc;
        cin >> vb >> vw >> vr >> vc;

        if (vr - vw - vb > 1e-9) {
            cout << "0\n";
            continue;
        }

        double l = max(0., vr - vw), r = min(vb, vc - vw), mid1, mid2;
        while (l + 1e-9 < r && l + l * 1e-9 < r) {
            mid1 = l + (r - l) / 3, mid2 = r - (r - l) / 3;

            auto remaining = [&](double m) {
                auto rem = vw / (vw + m);
                if (k > 1) {
                    auto rinse = min(vc - vr, (vb - m) / (k - 1));
                    for (int i = 1; i < k; i++) rem *= vr / (vr + rinse);
                }
                return rem;
            };

            if (remaining(mid1) > remaining(mid2)) l = mid1;
            else r = mid2;
        }

        cout << fixed << setprecision(8) << k << " " << r << " ";
        if (k > 1) r = min(vc - vr, (vb - l) / (k - 1));
        for (int i = 1; i < k; i++) cout << fixed << setprecision(8) << r << " ";
        cout << "\n";
    }
}
