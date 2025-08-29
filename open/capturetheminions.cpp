#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int mg, me, c, t;
    while (cin >> mg >> me >> c >> t && mg && me && c && t) {
        auto p = c / 6., ratio = (1. - p) / p;
        int r = !(mg % t) ? t : mg % t, x = (mg - r) / t + 1, y = (mg + me - 1 - r) / t + 2;
        cout << fixed << setprecision(1) << (c == 3 ? (double) x / y : ((pow(ratio, x) - 1) / (pow(ratio, y) - 1))) * 1e2 << "\n";
    }
}
