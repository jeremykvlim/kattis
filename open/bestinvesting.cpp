#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, y, d, w;
    cin >> r >> y >> d >> w;

    double rate = 1 + (double) r / 100, gain = 0;
    for (int i = 0, j = y; i <= y && ~j; i++, j--) {
        auto gi = pow(rate, i), gj = pow(rate, j), need = (1 - gi) / (1 - rate) * w / gi, have = (1 - gj) / (1 - rate) * d * rate;
        if (need <= have + d) gain = max(gain, (double) i * w - j * d + min((double) w, have - need));
    }
    cout << fixed << setprecision(6) << gain;
}
