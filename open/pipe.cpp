#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    cin >> c;

    while (c--) {
        int l, v1, v2, t, s;
        cin >> l >> v1 >> v2 >> t >> s;

        int remaining = 1, knocks = 0;
        for (int prev = (v2 - v1 + t - 1) / t, curr, offset = v2 - prev * t; prev > remaining && remaining > 0; remaining <<= 1, prev = curr) {
            curr = min(prev, (l / (++knocks * s) - offset + t) / t);
            remaining -= prev - curr;
        }

        if (remaining < 1) cout << "impossible\n";
        else cout << knocks << "\n";
    }
}
