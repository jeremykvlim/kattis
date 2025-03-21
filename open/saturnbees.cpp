#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int ntz = countr_zero((unsigned) n), mtz = countr_zero((unsigned) m);
    cout << ((mtz != 1 && ntz + mtz > 2 && ntz >= 1 && mtz >= 2) || (!ntz || !mtz) && ((countr_zero((unsigned) ((n + 1) & ~2)) < mtz) || (countr_zero((unsigned) ((m + 1) & ~2)) < ntz)) ? "possible" : "impossible");
}
