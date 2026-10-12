#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double b, c;
    cin >> b >> c;

    double lgb = log(b), l = 0, r = log1p(c / lgb) / lgb, m;
    while (l + 1e-7 < r && l + l * 1e-7 < r) {
        m = l + (r - l) / 2;

        if (lgb * exp(m * lgb) * (m + 1) * (m + 1) < c) l = m;
        else r = m;
    }
    cout << fixed << setprecision(5) << exp(m * lgb) + c / (m + 1);
}