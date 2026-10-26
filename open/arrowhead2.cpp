#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double l1, b1, l2, b2, x2;
    cin >> l1 >> b1 >> l2 >> b2 >> x2;

    auto m1 = b1 / l1, m2 = b2 / l2, w1 = min(l1 - x2, l2), area = l1 * b1 + l2 * b2 - m2 * w1 * w1;
    if (m1 < m2) {
        auto w2 = max(0., w1 - m1 * x2 / (m2 - m1));
        area += (m2 - m1) * w2 * w2;
    }
    cout << fixed << setprecision(6) << area / 2;
}