#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int ntz = __builtin_ctz(n), mtz = __builtin_ctz(m);
    cout << ((mtz != 1 && ntz + mtz > 2 && ntz >= 1 && mtz >= 2) || (!ntz || !mtz) && ((__builtin_ctz((n + 1) & ~2) < mtz) || (__builtin_ctz((m + 1) & ~2) < ntz)) ? "possible" : "impossible");
}
